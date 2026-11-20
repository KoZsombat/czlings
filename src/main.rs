//! czlings -- a rustlings-style exercise runner for C and C++.
//!
//! Run `czlings` (or `czlings watch`) from the repository root.  Exercises are
//! listed in `info.toml`; an exercise counts as done once its file no longer
//! contains the line `I AM NOT DONE`.

use std::env;
use std::fs;
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};
use std::thread::sleep;
use std::time::Duration;

const MARKER: &str = "I AM NOT DONE";
const RED: &str = "\x1b[31m";
const GREEN: &str = "\x1b[32m";
const YELLOW: &str = "\x1b[33m";
const BLUE: &str = "\x1b[34m";
const DIM: &str = "\x1b[2m";
const BOLD: &str = "\x1b[1m";
const RESET: &str = "\x1b[0m";

#[derive(Clone, Copy, PartialEq, Eq)]
enum Mode {
    Test,
    Compile,
}

struct Exercise {
    name: String,
    path: String,
    mode: Mode,
    hint: String,
}

#[cfg(unix)]
fn restore_sigpipe() {
    // Rust ignores SIGPIPE, which turns `czlings list | head` into a panic.
    // Restore the Unix default so a closed pipe terminates quietly.
    extern "C" {
        fn signal(signum: i32, handler: usize) -> usize;
    }
    const SIGPIPE: i32 = 13;
    const SIG_DFL: usize = 0;
    unsafe {
        signal(SIGPIPE, SIG_DFL);
    }
}

#[cfg(not(unix))]
fn restore_sigpipe() {}

fn main() -> ExitCode {
    restore_sigpipe();

    let args: Vec<String> = env::args().skip(1).collect();

    let Some(root) = find_root() else {
        eprintln!("{RED}czlings: cannot find info.toml{RESET}");
        eprintln!("Run this command from inside the tutorial repository.");
        return ExitCode::from(1);
    };
    let exercises = load_manifest(&root);
    if exercises.is_empty() {
        eprintln!("{RED}czlings: info.toml lists no exercises{RESET}");
        return ExitCode::from(1);
    }

    let command = args.first().map(String::as_str).unwrap_or("watch");
    let target = args.get(1).map(String::as_str);

    match command {
        "watch" | "w" => watch(&root, &exercises),
        "run" | "r" => match pick(&root, &exercises, target) {
            Some(ex) => {
                if run_one(&root, ex) {
                    ExitCode::SUCCESS
                } else {
                    ExitCode::from(1)
                }
            }
            None => {
                if target.is_none() {
                    ExitCode::SUCCESS
                } else {
                    ExitCode::from(1)
                }
            }
        },
        "list" | "l" => {
            list(&root, &exercises);
            ExitCode::SUCCESS
        }
        "hint" | "h" => hint(&root, &exercises, target),
        "reset" => reset(&root, &exercises, target),
        "solution" | "solve" => solution(&root, &exercises, target),
        "verify" => verify(&root, &exercises),
        "check" => check(&root, &exercises),
        "help" | "--help" | "-h" => {
            print_help();
            ExitCode::SUCCESS
        }
        other => {
            eprintln!("{RED}unknown command `{other}`{RESET}");
            print_help();
            ExitCode::from(1)
        }
    }
}

fn print_help() {
    println!(
        "{BOLD}czlings{RESET} -- learn C and C++ by fixing broken programs

{BOLD}USAGE{RESET}
  czlings                    watch mode: run the next unfinished exercise
  czlings run [NAME]         compile and run one exercise (or the next one)
  czlings list               show every exercise and its status
  czlings hint [NAME]        print the hint for an exercise
  czlings reset [NAME]       restore exercise file(s) from git
  czlings solution [NAME]    copy the reference solution over an exercise
  czlings verify             build and run every reference solution
  czlings check              validate the manifest and repository layout

Exercise names may be given in full or as a unique substring (e.g. `c07`).
Environment: CC, CXX (compilers), CZ_SAN=1 (AddressSanitizer + UBSan).{RESET}"
    );
}

// ---------------------------------------------------------------------------
// Repository layout
// ---------------------------------------------------------------------------

fn find_root() -> Option<PathBuf> {
    let mut dir = env::current_dir().ok()?;
    for _ in 0..8 {
        if dir.join("info.toml").is_file() && dir.join("exercises").is_dir() {
            return Some(dir);
        }
        if !dir.pop() {
            break;
        }
    }
    None
}

fn load_manifest(root: &Path) -> Vec<Exercise> {
    let path = root.join("info.toml");
    let text = match fs::read_to_string(&path) {
        Ok(text) => text,
        Err(err) => {
            eprintln!("cannot read {}: {err}", path.display());
            return Vec::new();
        }
    };

    let mut exercises = Vec::new();
    let mut current: Option<Exercise> = None;
    let mut seen: Vec<String> = Vec::new();

    for raw in text.lines() {
        let line = raw.trim();
        if line.is_empty() || line.starts_with('#') {
            continue;
        }
        if line == "[[exercises]]" {
            if let Some(ex) = current.take() {
                push_exercise(&mut exercises, &mut seen, ex);
            }
            current = Some(Exercise {
                name: String::new(),
                path: String::new(),
                mode: Mode::Test,
                hint: String::new(),
            });
            continue;
        }
        let Some((key, value)) = line.split_once('=') else {
            continue;
        };
        let Some(ex) = current.as_mut() else {
            continue;
        };
        match key.trim() {
            "name" => ex.name = parse_string(value),
            "path" => ex.path = parse_string(value),
            "mode" => {
                ex.mode = if parse_string(value).eq_ignore_ascii_case("compile") {
                    Mode::Compile
                } else {
                    Mode::Test
                }
            }
            "hint" => ex.hint = parse_string(value),
            _ => {}
        }
    }
    if let Some(ex) = current.take() {
        push_exercise(&mut exercises, &mut seen, ex);
    }
    exercises
}

fn push_exercise(out: &mut Vec<Exercise>, seen: &mut Vec<String>, ex: Exercise) {
    if ex.name.is_empty() || ex.path.is_empty() {
        eprintln!("{YELLOW}warning: skipping manifest entry with missing name/path{RESET}");
        return;
    }
    if seen.contains(&ex.name) {
        eprintln!("{YELLOW}warning: duplicate exercise name `{}`{RESET}", ex.name);
        return;
    }
    seen.push(ex.name.clone());
    out.push(ex);
}

fn parse_string(value: &str) -> String {
    let value = value.trim();
    if let Some(rest) = value.strip_prefix('"') {
        let mut out = String::new();
        let mut escape = false;
        for ch in rest.chars() {
            if escape {
                out.push(match ch {
                    'n' => '\n',
                    't' => '\t',
                    'r' => '\r',
                    '0' => '\0',
                    other => other,
                });
                escape = false;
            } else if ch == '\\' {
                escape = true;
            } else if ch == '"' {
                break;
            } else {
                out.push(ch);
            }
        }
        out
    } else {
        value.split('#').next().unwrap_or("").trim().to_string()
    }
}

fn solution_path(root: &Path, ex: &Exercise) -> PathBuf {
    let rel = ex
        .path
        .strip_prefix("exercises/")
        .unwrap_or(ex.path.as_str());
    root.join("solutions").join(rel)
}

fn is_done(root: &Path, ex: &Exercise) -> bool {
    match fs::read_to_string(root.join(&ex.path)) {
        Ok(text) => !text.contains(MARKER),
        Err(_) => false,
    }
}

fn is_cpp(ex: &Exercise) -> bool {
    matches!(
        Path::new(&ex.path).extension().and_then(|e| e.to_str()),
        Some("cpp" | "cc" | "cxx")
    )
}

// ---------------------------------------------------------------------------
// Picking exercises
// ---------------------------------------------------------------------------

fn pick<'a>(root: &Path, exercises: &'a [Exercise], target: Option<&str>) -> Option<&'a Exercise> {
    match target {
        Some(name) => {
            if let Some(ex) = exercises.iter().find(|ex| ex.name == name) {
                return Some(ex);
            }
            let hits: Vec<&Exercise> = exercises
                .iter()
                .filter(|ex| ex.name.contains(name))
                .collect();
            match hits.len() {
                1 => Some(hits[0]),
                0 => {
                    eprintln!("{RED}no exercise matches `{name}`{RESET}");
                    None
                }
                _ => {
                    let names: Vec<&str> = hits.iter().map(|ex| ex.name.as_str()).collect();
                    eprintln!(
                        "{RED}`{name}` is ambiguous, candidates: {}{RESET}",
                        names.join(", ")
                    );
                    None
                }
            }
        }
        None => {
            let next = exercises
                .iter()
                .find(|ex| !is_done(root, ex));
            if next.is_none() {
                println!(
                    "{GREEN}{BOLD}All {} exercises are complete.{RESET}",
                    exercises.len()
                );
            }
            next
        }
    }
}

// ---------------------------------------------------------------------------
// Building and running
// ---------------------------------------------------------------------------

fn build(root: &Path, ex: &Exercise, source: &Path) -> Result<(PathBuf, String), String> {
    let dir = root
        .join(".czlings")
        .join("build")
        .join(format!("{}", std::process::id()))
        .join(&ex.name);
    fs::create_dir_all(&dir).map_err(|err| err.to_string())?;
    let exe = dir.join("exercise");

    let cpp = is_cpp(ex);
    let compiler = if cpp {
        env::var("CXX").unwrap_or_else(|_| "c++".to_string())
    } else {
        env::var("CC").unwrap_or_else(|_| "cc".to_string())
    };

    let mut cmd = Command::new(&compiler);
    cmd.arg(if cpp { "-std=c++17" } else { "-std=c17" })
        .args(["-Wall", "-Wextra", "-g", "-I"])
        .arg(root.join("harness"));
    if env::var_os("CZ_SAN").is_some() {
        cmd.args(["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]);
    }
    cmd.arg(source).arg("-o").arg(&exe);
    if !cpp {
        cmd.arg("-lm");
    }

    let output = cmd
        .output()
        .map_err(|err| format!("failed to launch `{compiler}`: {err}"))?;
    let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
    if !output.status.success() {
        return Err(if stderr.trim().is_empty() {
            format!("{compiler} exited with {}", output.status)
        } else {
            stderr
        });
    }
    Ok((exe, stderr))
}

fn print_header(ex: &Exercise) {
    let mode = if ex.mode == Mode::Test { "test" } else { "compile" };
    println!(
        "{BOLD}{BLUE}{}{RESET}  {DIM}{}  [{mode}]{RESET}",
        ex.name, ex.path
    );
    println!();
}

fn run_one(root: &Path, ex: &Exercise) -> bool {
    print_header(ex);
    let source = root.join(&ex.path);
    if !source.is_file() {
        println!("{RED}missing exercise file: {}{RESET}", source.display());
        return false;
    }

    let has_marker = fs::read_to_string(&source)
        .map(|text| text.contains(MARKER))
        .unwrap_or(false);

    let (exe, warnings) = match build(root, ex, &source) {
        Ok(result) => result,
        Err(message) => {
            println!("{RED}{BOLD}compilation failed{RESET}");
            println!("{message}");
            return false;
        }
    };
    if !warnings.trim().is_empty() {
        println!("{DIM}{}{RESET}", warnings.trim_end());
    }

    if ex.mode == Mode::Compile {
        println!("{GREEN}compiled successfully{RESET}");
    } else {
        let output = match Command::new(&exe).output() {
            Ok(output) => output,
            Err(err) => {
                println!("{RED}failed to run exercise binary: {err}{RESET}");
                return false;
            }
        };
        print!("{}", String::from_utf8_lossy(&output.stdout));
        let stderr = String::from_utf8_lossy(&output.stderr);
        if !stderr.trim().is_empty() {
            println!("{RED}{}{RESET}", stderr.trim_end());
        }
        if !output.status.success() {
            println!("{RED}{BOLD}tests failed{RESET}");
            return false;
        }
    }

    println!();
    if has_marker {
        println!(
            "{YELLOW}All checks passed. Delete the `// {MARKER}` line to finish this exercise.{RESET}"
        );
    } else {
        println!("{GREEN}exercise complete{RESET}");
    }
    true
}

// ---------------------------------------------------------------------------
// Commands
// ---------------------------------------------------------------------------

fn watch(root: &Path, exercises: &[Exercise]) -> ExitCode {
    loop {
        let Some(ex) = exercises.iter().find(|ex| !is_done(root, ex)) else {
            println!(
                "{GREEN}{BOLD}All {} exercises are complete. Nice work.{RESET}",
                exercises.len()
            );
            return ExitCode::SUCCESS;
        };

        clear_screen();
        run_one(root, ex);

        let source = root.join(&ex.path);
        let mut stamp = mtime(&source);
        loop {
            sleep(Duration::from_millis(250));
            let now = mtime(&source);
            if now != stamp {
                stamp = now;
                sleep(Duration::from_millis(120));
                break;
            }
        }
    }
}

fn clear_screen() {
    // ANSI clear + move home; harmless in non-tty pipes.
    print!("\x1b[2J\x1b[H");
}

fn mtime(path: &Path) -> Option<std::time::SystemTime> {
    fs::metadata(path).and_then(|meta| meta.modified()).ok()
}

fn list(root: &Path, exercises: &[Exercise]) {
    let done = exercises.iter().filter(|ex| is_done(root, ex)).count();
    for (index, ex) in exercises.iter().enumerate() {
        let status = if is_done(root, ex) {
            format!("{GREEN}done{RESET}")
        } else {
            format!("{YELLOW}todo{RESET}")
        };
        println!(
            "{:>3}. {:<30} {}  {DIM}{}{RESET}",
            index + 1,
            ex.name,
            status,
            ex.path
        );
    }
    println!("\n{done}/{} complete", exercises.len());
}

fn hint(root: &Path, exercises: &[Exercise], target: Option<&str>) -> ExitCode {
    let Some(ex) = pick(root, exercises, target) else {
        return ExitCode::from(1);
    };
    println!("{BOLD}{}{RESET}", ex.name);
    if ex.hint.trim().is_empty() {
        println!("(no hint for this exercise)");
    } else {
        println!("{}", ex.hint);
    }
    ExitCode::SUCCESS
}

fn reset(root: &Path, exercises: &[Exercise], target: Option<&str>) -> ExitCode {
    if target.is_none() {
        print!(
            "Reset all {} exercises and lose your work? [y/N] ",
            exercises.len()
        );
        let _ = io::stdout().flush();
        let mut answer = String::new();
        if io::stdin().read_line(&mut answer).is_err()
            || !answer.trim().eq_ignore_ascii_case("y")
        {
            println!("aborted");
            return ExitCode::SUCCESS;
        }
    }

    let targets: Vec<&Exercise> = match target {
        Some(_) => match pick(root, exercises, target) {
            Some(ex) => vec![ex],
            None => return ExitCode::from(1),
        },
        None => exercises.iter().collect(),
    };

    let mut failures = 0;
    for ex in targets {
        let status = Command::new("git")
            .arg("checkout")
            .arg("--")
            .arg(&ex.path)
            .current_dir(root)
            .status();
        match status {
            Ok(status) if status.success() => println!("restored {}", ex.path),
            _ => {
                println!("{RED}could not restore {} (is this a git repo with a commit?){RESET}", ex.path);
                failures += 1;
            }
        }
    }
    if failures == 0 {
        ExitCode::SUCCESS
    } else {
        ExitCode::from(1)
    }
}

fn solution(root: &Path, exercises: &[Exercise], target: Option<&str>) -> ExitCode {
    let targets: Vec<&Exercise> = match target {
        Some(_) => match pick(root, exercises, target) {
            Some(ex) => vec![ex],
            None => return ExitCode::from(1),
        },
        None => exercises.iter().collect(),
    };

    let mut failures = 0;
    for ex in targets {
        let source = solution_path(root, ex);
        if !source.is_file() {
            println!("{RED}missing solution: {}{RESET}", source.display());
            failures += 1;
            continue;
        }
        match fs::copy(&source, root.join(&ex.path)) {
            Ok(_) => println!("copied solution over {}", ex.path),
            Err(err) => {
                println!("{RED}copy failed for {}: {err}{RESET}", ex.path);
                failures += 1;
            }
        }
    }
    if failures == 0 {
        ExitCode::SUCCESS
    } else {
        ExitCode::from(1)
    }
}

fn verify(root: &Path, exercises: &[Exercise]) -> ExitCode {
    println!("{BOLD}verifying reference solutions{RESET}\n");
    let mut failures = 0;

    for (index, ex) in exercises.iter().enumerate() {
        let source = solution_path(root, ex);
        if !source.is_file() {
            println!(
                "{:>3}. {:<30} {RED}missing solution{RESET}",
                index + 1,
                ex.name
            );
            failures += 1;
            continue;
        }
        match build(root, ex, &source) {
            Err(message) => {
                println!(
                    "{:>3}. {:<30} {RED}compile failed{RESET}",
                    index + 1,
                    ex.name
                );
                println!("{message}");
                failures += 1;
            }
            Ok((exe, warnings)) => {
                if !warnings.trim().is_empty() {
                    println!("{DIM}warnings in {}:{RESET}\n{}", ex.name, warnings.trim_end());
                }
                if ex.mode == Mode::Compile {
                    println!("{:>3}. {:<30} {GREEN}ok{RESET}", index + 1, ex.name);
                    continue;
                }
                let output = Command::new(&exe).output();
                match output {
                    Ok(output) if output.status.success() => {
                        println!("{:>3}. {:<30} {GREEN}ok{RESET}", index + 1, ex.name);
                    }
                    Ok(output) => {
                        println!(
                            "{:>3}. {:<30} {RED}checks failed{RESET}",
                            index + 1,
                            ex.name
                        );
                        print!("{}", String::from_utf8_lossy(&output.stdout));
                        let stderr = String::from_utf8_lossy(&output.stderr);
                        if !stderr.trim().is_empty() {
                            println!("{}", stderr.trim_end());
                        }
                        failures += 1;
                    }
                    Err(err) => {
                        println!(
                            "{:>3}. {:<30} {RED}cannot run: {err}{RESET}",
                            index + 1,
                            ex.name
                        );
                        failures += 1;
                    }
                }
            }
        }
    }

    if failures == 0 {
        println!("\n{GREEN}{BOLD}all {count} solutions verified{RESET}", count = exercises.len());
        ExitCode::SUCCESS
    } else {
        println!("\n{RED}{BOLD}{failures} of {count} solutions failed{RESET}", count = exercises.len());
        ExitCode::from(1)
    }
}

fn check(root: &Path, exercises: &[Exercise]) -> ExitCode {
    println!("{BOLD}checking repository layout{RESET}\n");
    let mut problems = 0;

    for ex in exercises {
        let source = root.join(&ex.path);
        if !source.is_file() {
            println!("{RED}missing exercise file: {}{RESET}", ex.path);
            problems += 1;
        }
        if !solution_path(root, ex).is_file() {
            println!("{RED}missing solution file: {}{RESET}", solution_path(root, ex).display());
            problems += 1;
        }
        if !source.is_file() {
            continue;
        }
        match fs::read_to_string(&source) {
            Ok(text) => {
                let markers = text.matches(MARKER).count();
                if markers != 1 {
                    println!(
                        "{YELLOW}{}: expected exactly one `{MARKER}` line, found {markers}{RESET}",
                        ex.path
                    );
                    problems += 1;
                }
            }
            Err(err) => {
                println!("{RED}cannot read {}: {err}{RESET}", ex.path);
                problems += 1;
            }
        }
        if let Ok(solution_text) = fs::read_to_string(solution_path(root, ex)) {
            if solution_text.contains(MARKER) {
                println!(
                    "{RED}{}: reference solution still contains `{MARKER}`{RESET}",
                    solution_path(root, ex).display()
                );
                problems += 1;
            }
        }
    }

    let mut found = Vec::new();
    collect_sources(&root.join("exercises"), &mut found);
    let listed: Vec<PathBuf> = exercises
        .iter()
        .map(|ex| root.join(&ex.path))
        .collect();
    for path in &found {
        if !listed.contains(path) {
            println!(
                "{YELLOW}exercise file not listed in info.toml: {}{RESET}",
                path.strip_prefix(root).unwrap_or(path).display()
            );
            problems += 1;
        }
    }

    if problems == 0 {
        println!("{GREEN}layout looks good ({count} exercises){RESET}", count = exercises.len());
        ExitCode::SUCCESS
    } else {
        println!("\n{RED}{problems} problem(s) found{RESET}");
        ExitCode::from(1)
    }
}

fn collect_sources(dir: &Path, out: &mut Vec<PathBuf>) {
    let Ok(entries) = fs::read_dir(dir) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            collect_sources(&path, out);
        } else if matches!(
            path.extension().and_then(|ext| ext.to_str()),
            Some("c" | "cpp" | "cc" | "cxx")
        ) {
            out.push(path);
        }
    }
}
