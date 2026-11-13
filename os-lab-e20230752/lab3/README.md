# OS Lab 3 — Wildcards, Hard Links and Symbolic Links

**Student ID:** e20230752  
**Student:** Din Mengsea  
**Environment:** Ubuntu Linux  
**Date:** November 13, 2026

## 1. Lab Objectives

The objectives of this lab are:

1. Understand how Bash wildcards select filenames and how quoting affects wildcard expansion.
2. Understand the difference between hard links and symbolic links using inode numbers.
3. Observe what happens to symbolic links when their target files are renamed.
4. Repair a broken symbolic link using a relative path.
5. Understand how multiple symbolic links can depend on one another.

## 2. Core 1 — Wildcards on Your Own Files

In this activity, I explored how Bash wildcard patterns select filenames in the `notes/` directory.

My personal filename prefix was `bridge`.

The directory contained:

- `bridge1.txt`
- `bridge2.txt`
- `bridge12.txt`
- `bridge.log`
- `.bridge.txt` (hidden file)

I executed three `printf` commands to compare wildcard expansion.

**Command 1:**

```bash
printf '<%s>\n' notes/*.txt
```

Output:

```text
<notes/bridge12.txt>
<notes/bridge1.txt>
<notes/bridge2.txt>
```

**Command 2:**

```bash
printf '<%s>\n' notes/"$name"?.txt
```

Output:

```text
<notes/bridge1.txt>
<notes/bridge2.txt>
```

The `?` wildcard matches exactly one character. Therefore, it matches `bridge1.txt` and `bridge2.txt`, but not `bridge12.txt`.

**Command 3:**

```bash
printf '<%s>\n' 'notes/*.txt'
```

Output:

```text
<notes/*.txt>
```

Because the pattern is enclosed in single quotes, Bash does not expand the wildcard. It prints the pattern literally.

**Observation:**

Bash expands unquoted wildcard patterns before executing a command. The `*` matches zero or more characters, while `?` matches exactly one character. Hidden files are not included when using the ordinary `*` pattern.

## 3. Core 2 — Rename, Break, and Reuse the Old Name

In this activity, I created a hard link and a symbolic link to my original report.

My personal values were:

- Name: `bridge`
- Tag: `draft`
- Original version: `192`
- New version: `203`

### 3.1 Before Renaming

I created the two links using:

```bash
ln "$name.txt" "$name-hard.txt"
ln -s "$name.txt" "$name-soft.txt"
```

The output of `ls -li` showed that `bridge.txt` and `bridge-hard.txt` shared the same inode number: `18750537`.

The symbolic link `bridge-soft.txt` had a different inode number: `18745021`.

Both links initially displayed:

```text
version 192
```

The symbolic link stored the target path:

```text
bridge.txt
```

**Observation:**

A hard link shares the same inode as the original file, while a symbolic link has its own inode and stores the pathname of its target.

### 3.2 After Renaming

I renamed the original report:

```bash
mv -- "$name.txt" "$name-$tag.txt"
```

The new filename became:

```text
bridge-draft.txt
```

After renaming, the hard link still worked and displayed:

```text
version 192
```

However, the symbolic link failed with:

```text
cat: bridge-soft.txt: No such file or directory
```

This happened because the symbolic link still stored `bridge.txt`, but that filename no longer existed.

**Observation:**

Renaming a file does not affect its inode. Therefore, the hard link continues to access the original data. However, the symbolic link becomes dangling because its stored pathname no longer points to an existing file.

### 3.3 Creating a New File Using the Old Name

I created a new file using the original filename:

```bash
printf 'version %s\n' "$v2" > "$name.txt"
```

The new file contained:

```text
version 203
```

The results were:

| Filename | Content |
|---|---|
| bridge.txt | version 203 |
| bridge-hard.txt | version 192 |
| bridge-soft.txt | version 203 |

**Observation and Surprise:**

The surprising result was that the symbolic link started working again after a new file was created using the old filename, `bridge.txt`. However, it displayed `version 203` instead of `version 192`.

This happened because a symbolic link stores a pathname rather than an inode number. When a new file was created with the same pathname, the symbolic link accessed the new file. In contrast, the hard link continued accessing the original file through its inode and displayed `version 192`.

## 4. Core 3 — Repair and Prove It

In this activity, I repaired the symbolic link so that it pointed to the original report again.

First, I deleted the old symbolic link:

```bash
rm -- bridge-soft.txt
```

Then, I created a new symbolic link with the correct relative target:

```bash
ln -s bridge-draft.txt bridge-soft.txt
```

I verified the repair using:

```bash
cat bridge-soft.txt
```

The output was:

```text
version 192
```

I also checked the stored target:

```bash
readlink bridge-soft.txt
```

Output:

```text
bridge-draft.txt
```

**Decision:**

I used a relative target because it allows the symbolic link to locate its target using the relative directory structure. If I used an absolute path and moved the entire folder to another location, the symbolic link might become broken because the original absolute path would no longer exist.

## 5. Plus — One More Pattern

In this optional activity, I explored the `[12]` and `[!1]` wildcard patterns.

**First command:**

```bash
printf '<%s>\n' notes/"$name"[12].txt
```

Output:

```text
<notes/bridge1.txt>
<notes/bridge2.txt>
```

The `[12]` pattern matches exactly one character, either `1` or `2`.

**Second command:**

```bash
printf '<%s>\n' notes/"$name"[!1]*
```

Output:

```text
<notes/bridge2.txt>
<notes/bridge.log>
```

The `[!1]` pattern matches one character that is not `1`, followed by zero or more characters.

**Observation:**

The first pattern selected files with either `1` or `2` as the next character after `bridge`. The second pattern selected filenames whose next character was not `1`, followed by any number of characters.

## 6. Prediction and Debrief

Before the experiment, I answered the prediction questions.

| Question | My prediction | Correct answer |
|---|---|---|
| Can the symbolic link read the file after renaming? | No | No |
| What version does the hard link display after creating a new file? | 203 | 192 |
| What version does the symbolic link display after creating a new file? | 203 | 203 |

My prediction for the second question was incorrect. I initially thought the hard link would display `version 203`. After performing the experiment, I understood that a hard link continues pointing to the original inode even when the original filename is renamed.

My prediction for the first and third questions was correct.

For the final prediction question, I explained:

A symbolic link points to a filename, while a hard link points directly to the inode of the original file.

After the experiment, I completed the debrief and corrected my understanding of the differences between hard links and symbolic links.

## 7. Live Checkpoint

In the live checkpoint, I worked with another set of files:

- `ocean.txt`
- `links/hard.txt`
- `links/first.txt`
- `links/second.txt`

The original file was renamed from `ocean.txt` to `ocean-final.txt`.

I answered the five checkpoint questions:

1. Does `links/hard.txt` still read the data? **Yes.**
2. Does `links/second.txt` read the data after the rename? **No.**
3. What relative target should `links/first.txt` store? **`../ocean-final.txt`**
4. After repairing `first.txt`, does `second.txt` read the data? **Yes.**
5. From which folder does Linux read the stored target of `links/first.txt`? **The `links/` folder.**

I repaired the symbolic link using:

```bash
ln -s ../ocean-final.txt links/first.txt
```

After repairing it, both the hard link and the symbolic link chain successfully accessed the original data.

The output was:

```text
checkpoint market ocean
checkpoint market ocean
```

I saved the terminal output in `evidence/checkpoint.txt`.

## 8. Conclusion

Through this lab, I gained practical experience with Bash wildcards, hard links, symbolic links, and inode numbers.

I learned that hard links refer directly to the same inode, while symbolic links store pathnames. I also learned that renaming a file can make a symbolic link dangling, and creating a new file with the old name can cause that symbolic link to access different content.

Finally, I practiced repairing symbolic links using relative paths and learned how one symbolic link can depend on another.

This lab helped me better understand how Linux manages filenames, inodes, and file references.
