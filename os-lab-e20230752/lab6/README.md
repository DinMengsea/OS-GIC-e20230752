# OS Lab 6 — Linux Security, Users, Groups & File Permissions

**Student ID:** e20230752
**Username:** DIN MENGSEA
**Lab:** 6 — Linux Security, Users, Groups & File Permissions

## 1. Lab Objectives

The objectives of this lab are:

- Understand Linux file and directory permissions.
- Interpret owner, group, and others permission bits.
- Apply permissions using octal and symbolic notation.
- Understand directory traversal and search permissions.
- Test file access and diagnose permission errors.
- Understand how `umask` affects newly created files.

## 2. Guided — Permission Basics

First, I checked my Linux identity using:

```bash
id
```

My user ID and group ID were both 1014.

I created a demo file and tested two different permission modes.

### Test 1: Octal permission 600

```bash
chmod 600 demo/note.txt
```

Output:

```text
-rw------- 600 demo/note.txt
```

Only the owner can read and write the file.

### Test 2: Symbolic permission 640

```bash
chmod u=rw,g=r,o= demo/note.txt
```

Output:

```text
-rw-r----- 640 demo/note.txt
```

The owner has read and write permissions, the group has read permission, and others have no permissions.

## 3. Core 1 — Set the Modes

### 3.1 Private Record Policy

The policy requires that only the owner can read and modify `private/record.txt`, and nobody else can enter the private directory.

I applied these permissions:

| Item | Octal Mode | Symbolic Mode |
|---|---:|---|
| private/ | 700 | rwx------ |
| private/record.txt | 600 | rw------- |

Commands:

```bash
chmod 700 private/
chmod 600 private/record.txt
```

### 3.2 Shared Notice Policy

My personal notice policy states:

"The owner and the owner's group may read and change the file and the folder. Others have no access."

I calculated and applied these permissions:

| Item | Octal Mode | Symbolic Mode |
|---|---:|---|
| shared/ | 770 | rwxrwx--- |
| shared/notice.txt | 660 | rw-rw---- |

Commands:

```bash
chmod 770 shared/
chmod 660 shared/notice.txt
```

### 3.3 Verification

I verified all four permissions using:

```bash
stat -c '%A %a %U:%G %n' private private/record.txt shared shared/notice.txt
```

All four permission modes matched the required policies.

## 4. Core 2 — Prove It

### 4.1 Private Record Test

I read the private record and appended an update:

```bash
cat private/record.txt
printf 'owner update\n' >> private/record.txt
```

I also checked whether the file had execute permission:

```bash
if test -x private/record.txt; then
    printf 'unexpected execute bit\n'
else
    printf 'not executable\n'
fi
```

Output:

```text
not executable
```

This confirms that the private record is not executable.

### 4.2 Directory Traversal Test

I created my personal demo directory, `sealed-panda`, and a file inside it.

```bash
mkdir -p demo/sealed-panda
printf 'inside\n' > demo/sealed-panda/item.txt
```

I changed the directory permission to 600:

```bash
chmod 600 demo/sealed-panda
```

I attempted to list the directory:

```bash
ls demo/sealed-panda
```

Output:

```text
item.txt
```

Then I attempted to read the file:

```bash
cat demo/sealed-panda/item.txt
```

Output:

```text
cat: demo/sealed-panda/item.txt: Permission denied
```

The reason is that directory mode 600 provides read permission but does not provide execute (search) permission.

After restoring the directory permission to 700:

```bash
chmod 700 demo/sealed-panda
cat demo/sealed-panda/item.txt
```

Output:

```text
inside
```

This experiment demonstrates that directory execute permission is required to access files inside a directory by name.

### 4.3 Hypothetical Peer Access

I inspected the complete file path using:

```bash
namei -l "$PWD/shared/notice.txt"
```

The output showed that the `oslab-work` and `lab6` directories have permission 700.

Although the shared notice has permission 660, another student cannot reach it through these directories because they do not have permission to traverse those parent directories.

This is a hypothetical access analysis based on the observed permissions, not an actual cross-user experiment.

## 5. Plus — Symbolic chmod

I used symbolic notation to set the shared notice file to the same permission mode, 660.

Command:

```bash
chmod u=rw,g=rw,o= shared/notice.txt
```

Verification:

```bash
stat -c '%A %a %n' shared/notice.txt
```

Output:

```text
-rw-rw---- 660 shared/notice.txt
```

This confirms that symbolic notation produces the same permission mode as octal notation.

## 6. Challenge — umask

The challenge demonstrates how `umask` affects permissions when creating new files.

I executed:

```bash
( umask 077; touch demo/umask-test.txt; stat -c '%A %a %n' demo/umask-test.txt )
```

Output:

```text
-rw------- 600 demo/umask-test.txt
```

Explanation:

- Default permissions for a new file: 666.
- Umask: 077.
- Resulting file permissions: 600.

A new directory created with the same umask would have permission 700.

Parentheses were used to run the command in a subshell so that the umask change would not affect my normal terminal session.

## 7. Evidence Files

The following evidence files are included in the `evidence/` directory:

| File | Description |
|---|---|
| policy.txt | Verification of the four required permission modes |
| traversal.txt | Directory traversal experiment and permission error |
| plus.txt | Symbolic chmod command and verification |
| challenge.txt | umask 077 experiment and result |

## 8. Conclusion

In this lab, I learned how Linux manages file and directory permissions using owner, group, and others.

I practiced setting permissions with both octal and symbolic notation. I also learned that directory execute permission is necessary to access files inside a directory by name, even when the file itself allows reading.

Finally, I tested how umask affects the permissions of newly created files and understood the importance of least-privilege access.
