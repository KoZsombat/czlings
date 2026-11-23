# Prog1 / Prog2 coverage map

This maps the BME VIK Prog1 and Prog2 lecture topics to material in this
repository. Use it as a checklist, not as a replacement for the official
lectures, labs and homework.

The lecture titles below come from `Info.zip`:

- Prog1 2021 site: `Info/1. félév/Prog 1/Segédanyagok/Site/prog1_2021/`
- Prog2 2025 slides: `Info/2. félév/Prog 2/Előadásdiák 2025/`
- Course notes: `Info/1. félév/Prog 1/Segédanyagok/c_jegyzet.pdf`,
  `Info/2. félév/Prog 2/Segédanyagok/Levendovszky - Szoftverfejlesztés C++ nyelven.pdf`

## Prog1 (C)

| Lecture | Topic | Covered by |
| --- | --- | --- |
| ea01 | Bevezető, algoritmusok | `b01`–`b10`, `p00_temperature`, `p00_receipt` |
| ea02 | Nevezetes algoritmusok, tömbök | `c10`, `c29`, `p01_gradebook`, `p02_c_matrix` |
| ea03 | Függvények, struktúrák | `c08`, `c17`, `p00_receipt` |
| ea04 | Pointerek, sztringek | `c13`–`c16`, `p02_log_analyzer`, `p02_contact_book` |
| ea05 | Tömbök algoritmusai, rekurzió | `c09`, `c29`, `p02_c_backtracking` |
| ea06 | Operátorok, számábrázolás | `c02`, `c03`, `c26` |
| ea07 | Dinamikus memóriakezelés | `c20`, `c21`, `p02_log_analyzer`, `p02_c_matrix`, `p02_c_bst` |
| ea08 | Program és külvilág | `c23`, `c24`, `c27`, `p02_log_analyzer` |
| ea09 | (site page missing) file/preprocessor topics | `c23`–`c25` |
| ea10 | Dinamikus adatszerkezetek I — listák | `c22`, `p02_contact_book` |
| ea11 | Dinamikus adatszerkezetek II — bináris fák | `p02_c_bst` |
| ea12 | Állapotgépek, reguláris kifejezések | `p02_c_state_machine` |
| ea13 | Generikus algoritmusok, visszalépő keresés | `c29`, `p02_c_backtracking` |
| ea14 | Nagyobb program: mini táblázatkezelő | `p02_c_expression`, `p02_c_matrix` |

Still course-specific:

- the actual lab tasks and NHF/homework specification;
- the exact NZH task wording and output format;
- lecturer-specific extras from the current semester.

## Prog2 (C++)

| Lecture | Topic | Covered by |
| --- | --- | --- |
| cpp01 | Bevezetés | `b`/`c` exercises, `p00_*`, `p01_*` |
| cpp02 | Névterek, memória- és kivételkezelés, függvénysablonok | `cpp14`, `cpp17`, `cpp19`, `p03_bank`, `p03_matrix` |
| cpp03 | OO paradigmák, osztályok | `cpp04`–`cpp06`, `p03_bank` |
| cpp04 | Konstruktorok, értékadás, dinamikus szerkezetek | `cpp05`, `cpp07`, `cpp18`, `cpp30`, `p03_matrix` |
| cpp05 | Objektummodell, öröklés, virtuális tagfüggvények | `cpp11`, `cpp12`, `cpp35`, `p03_bank`, `p03_cpp_library` |
| cpp06 | Mutatókonverziók, heterogén kollekciók | `cpp12`, `p03_bank`, `p03_cpp_library` (`dynamic_cast`, `vector<unique_ptr<Base>>`) |
| cpp07 | Generikus szerkezetek | `cpp19`–`cpp21`, `p04_catalog` (`std::variant`) |
| cpp08 | Bejárók, tervezési minták | `cpp26`, `p04_cpp_pipeline` (custom iterator, Observer) |
| cpp09 | Többszörös öröklés, cast, perzisztencia | `p03_cpp_library` |
| cpp10 | STL | `cpp22`–`cpp26`, `p04_word_index`, `p04_catalog` |

Still course-specific:

- the Moodle ZH1 theory bank (`Info/2. félév/Prog 2/Zárthelyi/Prog2 igaz-hamis.xlsx`);
- the exact ZH2 coding tasks and allowed helpers;
- the lab/NHF requirements and any lecturer-specific design patterns.

## What is intentionally not a project

These are study/practice material, not single-file projects:

- the official ZH PDFs (use them under a timer);
- the Moodle true/false bank (use the spreadsheet);
- the lab and NHF specifications (use the files under
  `Info/1. félév/Prog 1/Bulk Prog. 1/Labor/` and
  `Info/2. félév/Prog 2/`);
- maths, physics, digital design and electronics (outside this repository).
