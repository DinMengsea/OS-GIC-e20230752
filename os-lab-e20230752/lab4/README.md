# OS Lab 4 — Linux I/O Redirection and Pipelines

- **Student Name:** DIN MENGSEA
- **Student ID:** e20230752
- **Ubuntu Username:** gic-din-mengsea
- **My Personal Values:** status = `paid`, other = `sent`

---

## 1. Lab Objectives

The objectives of this lab are:

1. Understand standard input, standard output, and standard error.
2. Practice output redirection using `>`, `>>`, and `2>`.
3. Use pipelines to connect commands.
4. Use `grep` to select matching records, `cut` to extract fields, and `wc -l` to count lines.
5. Understand how to handle missing files and save error messages separately.

## 2. Core 1 — Select, Extract and Count

### Part A — Select Matching Rows

First, I read the `data/events.csv` file and identified the records with my personal status word, `paid`.

I used the following command:

```bash
grep '^paid,' data/events.csv | tee evidence/selected.txt
```

**Output:**

```text
paid,5
paid,7
paid,5
```

The `grep '^paid,'` command selects only lines that start with `paid,`. The `tee` command displays the output and saves the same output into `evidence/selected.txt`.

### Part B — Extract Quantities

I used `cut` to extract the second column from the matching records.

```bash
grep '^paid,' data/events.csv | cut -d, -f2 | tee evidence/fields.txt
```

**Output:**

```text
5
7
5
```

- `-d,` specifies the comma as the delimiter.
- `-f2` selects the second field.
- `tee` displays and saves the extracted quantities.

### Part C — Count Matching Records

I used `wc -l` to count the number of matching records.

```bash
grep '^paid,' data/events.csv | wc -l > evidence/normal.txt
```

To check the result:

```bash
cat evidence/normal.txt
```

**Output:**

```text
3
```

I also compared the result with the expected count:

```bash
diff -u data/expected.txt evidence/normal.txt
```

The command produced no output, which means the two files contained matching results.

**Explanation:**

The `wc -l` command counts the number of matching lines, not the sum of the quantities. There are three matching records, so the result is 3 rather than 17.

---

## 3. Core 2 — Changed Inputs and Missing Files

### Part A — No-Match Input

I created a CSV file containing no records with my status word, `paid`.

```bash
printf 'status,count\nsent,2\n' > data/no-match.csv
```

Then I counted the matching records:

```bash
grep '^paid,' data/no-match.csv | wc -l > evidence/edge.txt
```

**Output:**

```text
0
```

This shows that there are no matching records in the file.

### Part B — Changed Input

First, I copied the original CSV file:

```bash
cp -- data/events.csv data/changed.csv
```

Then, I added two records:

```bash
printf 'paid,7\npaid-late,99\n' >> data/changed.csv
```

I counted the matching records:

```bash
grep '^paid,' data/changed.csv | wc -l >> evidence/edge.txt
```

**Output of `cat evidence/edge.txt`:**

```text
0
4
```

**Explanation:**

The original CSV contained three matching `paid` records. I added one more exact matching record, `paid,7`, making four matching records.

The record `paid-late,99` was not selected because it does not start with the exact pattern `paid,`.

I used `>` to replace the file content for the first result and `>>` to append the second result without deleting the first one.

### Part C — Missing File and Error Redirection

I tested what happens when `cat` tries to read a file that does not exist.

```bash
cat data/missing.csv > evidence/stdout.txt 2> evidence/stderr.txt
```

Immediately after the command, I saved its exit status:

```bash
echo $? > evidence/status.txt
```

**Standard output (`stdout.txt`):**

The file was empty because `cat` could not read the missing file.

**Standard error (`stderr.txt`):**

```text
cat: data/missing.csv: No such file or directory
```

**Exit status (`status.txt`):**

```text
1
```

**Explanation:**

Linux uses three standard streams:

- Stream 0: Standard input (stdin).
- Stream 1: Standard output (stdout).
- Stream 2: Standard error (stderr).

The missing-file diagnostic was sent to stream 2. The exit status was 1, indicating that the command failed.

---

## 4. Instructor Demonstration, Plus and Challenge

### Pipeline Demonstration

The command:

```bash
cat data/missing.csv | wc -l
```

demonstrates that a pipeline can hide an error if we only check the final command's exit status.

`cat` generates an error because the file does not exist, while `wc -l` receives no input and successfully counts zero lines. Therefore, the pipeline normally returns status 0 because `wc -l` is the last command.

This shows why checking only the pipeline's exit status may not reveal an earlier command failure.

### Challenge — Display and Save Both Streams

I used the following command:

```bash
cat data/events.csv data/missing.csv 2>&1 | tee evidence/challenge.txt
```

**Output:**

```text
status,count
paid,5
sent,8
paid,7
sent,5
paid,5
paid-late,63
cat: data/missing.csv: No such file or directory
```

I verified the saved output using:

```bash
cat evidence/challenge.txt
```

The file contained both the CSV contents and the missing-file diagnostic.

**Explanation:**

The `2>&1` operator redirects standard error into standard output. This allows `tee` to receive and save both normal output and error messages.

Without `2>&1`, `tee` would only save standard output, while the error message would remain on the terminal.

---

## 5. Live Checkpoint

### Part A — Checkpoint Questions

I was given the following records in `data/stone.csv`:

```csv
status,count
sent,5
paid,11
paid-late,34
paid,4
not-paid,58
```

My answers were:

1. Number of records selected by `grep '^paid,'`: **2**
2. First quantity extracted by `cut -d, -f2`: **11**
3. Stream carrying the missing-file diagnostic: **2 (stderr)**
4. Explanation: A record count counts matching rows, while a sum of quantities adds the numerical values in the second column.

### Part B — Save Matching Records

I used my personal status word, `paid`, to select the matching records:

```bash
grep '^paid,' data/stone.csv | tee evidence/checkpoint-selected.txt
```

**Output:**

```text
paid,11
paid,4
```

### Part C — Save the Record Count

I used `wc -l` to count the matching records and saved the result:

```bash
grep '^paid,' data/stone.csv | wc -l > checkpoint.txt
```

**Content of `checkpoint.txt`:**

```text
2
```

This confirms that there are two records matching my personal status word.

---

## 6. Debrief — What I Learned

Through this lab, I learned the following concepts:

1. **Pipelines (`|`):** A pipeline passes the standard output of one command as the standard input of another command.

2. **Output redirection (`>`):** It saves standard output into a file and replaces the existing contents.

3. **Append redirection (`>>`):** It adds new output to the end of a file without removing previous contents.

4. **Error redirection (`2>`):** It saves error messages separately from normal output.

5. **The `tee` command:** It displays output on the terminal while saving the same output into a file.

6. **The `wc -l` command:** It counts lines rather than adding numerical quantities.

7. **Exit status:** A status of 0 means success, while a nonzero status indicates a different outcome. For example, `grep` returns 1 when no matching line is found.

This lab helped me understand how Linux handles input, output, errors, and pipelines through practical command-line exercises.

---

## 7. Conclusion

In conclusion, this lab helped me develop a better understanding of Linux input/output redirection and pipelines through practical exercises. I learned how to use `grep`, `cut`, `wc`, and `tee` to select records, extract information, count matching lines, and save command outputs. I also learned the difference between standard output and standard error, how to handle missing files, and how to use exit statuses to identify command failures. Overall, this lab improved my confidence in using Linux command-line tools and taught me how to manage command outputs and errors more effectively.
