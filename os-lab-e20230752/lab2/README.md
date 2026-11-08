# OS Lab 2 Submission — Linux Navigation & File Management

- **Student Name:** DIN MENGSEA
- **Student ID:** e20230752
- **Ubuntu username on the server:** gic-din-mengsea
- **My values** (`oslab values lab2`):
  - `file1` = `vendor list.txt`
  - `file2` = `cash flow.txt`
  - `owner` = `Legal`
  - `reader` = `HR`
  - `third` = `Engineering`
  - `amount1` = `713`
  - `amount2` = `800`

---

## Task 1: Directory Creation and File Navigation

I created the department directory structure using the `mkdir -p` command. The structure included the Legal, HR, and Engineering departments, as well as an archive directory inside Legal.

I used the `cd` command to navigate between directories and `ls` to inspect their contents. I also used `cat` to read the two incoming files and understand their contents.

The contents were:

- `vendor list.txt`: `amount,713`
- `cash flow.txt`: `amount,800`

This task helped me understand how to create nested directories and navigate through the Linux file system.

## Task 2: Moving and Copying Files

I used the `mv` command to move both files from the `incoming` directory into the `reports` directory.

I practised two approaches:

- **Relative path:** I moved the first file using `../reports/`.
- **Absolute path:** I moved the second file using the full path stored in `$OSLAB_WORKSPACE`.

After moving the files, I used the `cp` command to copy both reports into the Legal department. I also copied the first report into the archive directory and renamed the copy to `first-original.txt`.

I learned that `mv` moves or renames files, while `cp` creates another copy without removing the original file.

## Task 3: Relative Paths and Absolute Paths

I navigated to the HR directory and accessed the Legal report using both relative and absolute paths.

The relative path was:

`../Legal/vendor list.txt`

The absolute path was:

`$OSLAB_WORKSPACE/lab2/TechCorp/Legal/vendor list.txt`

Both commands successfully displayed the file contents.

I learned that a relative path depends on the current working directory, while an absolute path starts from the root directory and identifies the complete location of a file.

## Task 4: Comparing Files and Correcting a Path Error

I used the `cmp` command to compare the original reports in the `reports` directory with their copies in the Legal directory.

Both comparisons completed without output, showing that the corresponding files were identical.

I also intentionally tried to access a report using an incorrect relative path from the HR directory:

`cat "reports/vendor list.txt"`

The command failed because there was no `reports` directory inside HR.

I corrected the path to:

`../../reports/vendor list.txt`

This worked because I needed to move up two directory levels from HR to reach the Lab 2 directory, then enter `reports`.

This task helped me understand how to identify and fix relative path errors.

## Plus: Moving and Renaming a File

I created a nested directory structure and a file named `lost file.txt`.

From the `mess/c` directory, I moved and renamed the file in one command:

`mv -- ../a/b/'lost file.txt' found.txt`

I then copied `found.txt` into the Engineering department.

I learned that a single `mv` command can move and rename a file at the same time.

## Challenge: Wildcards and Hidden Files

I used the wildcard pattern `*.txt` to copy text files from the `reports` directory into Engineering.

The command was:

`cp -- reports/*.txt TechCorp/Engineering/`

I learned two important things:

1. If I put quotes around `"*.txt"`, the shell does not expand the wildcard. It treats it as a literal filename.
2. Hidden files beginning with a dot, such as `.hidden.txt`, are not matched by `*.txt` by default.

This helped me understand how shell wildcard expansion works and how Linux handles hidden files.

---

## Evidence

The `evidence` directory contains the following files:

- `paths.txt` — records the successful file operations, path examples, and the intentional path error with its correction.
- `tree.txt` — records the final file and directory structure.
- `plus.txt` — records the additional Plus task commands and path explanation.
- `challenge.txt` — records the wildcard and hidden-file observations.
- `checkpoint.txt` — Live Checkpoint evidence, to be added if saved during the checkpoint.

