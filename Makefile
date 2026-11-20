.PHONY: build install manifest check verify test clean

build:
	cargo build --release

install:
	cargo install --path . --force

# info.toml is generated from the per-chapter fragments so that content
# batches never edit the same file.
manifest:
	cat manifest.d/*.toml > info.toml

check: manifest
	cargo run --release -- check

verify: manifest
	cargo run --release -- verify

test: verify

clean:
	cargo clean
	rm -rf .czlings
