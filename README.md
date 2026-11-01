# Bytebox

**Bytebox** is a lightweight command-line database written in C++ with a simple plain-text storage format and intuitive commands for working with tables right from the terminal.

The project needs no server, dependencies, or installation: a single executable and a text database file are enough.

## Features

- Data stored in a plain text file (values separated by spaces)
- Read the table with aligned columns
- Add, modify, and delete rows, columns, and individual cells
- Search by value and filter by boolean conditions (`and`, `or`, `not`, parentheses)
- Numeric and string comparison, case-insensitive search, substring matching
- Sort by one or several columns
- Find unique values and rows with occurrence counts
- Bulk replacement of values
- Run scripts from a file with `if / elif / else` support
- Switch between database files

## Build

A compiler with C++17 support is required (`std::filesystem` is used).

```bash
g++ main.cpp -o bytebox
```

## Quick start

```bash
./bytebox add name age city      # the first row becomes the header
./bytebox add Alice 30 Berlin
./bytebox add Bob 25 London
./bytebox read
```

Output:

```
id | name  | age | city
1  | Alice | 30  | Berlin
2  | Bob   | 25  | London
```

Each `bytebox` invocation executes one command. If the command modifies data, the database is saved automatically.

## Database format

- The database is a text file, `database.db` by default.
- The first line is the header (row `0`); data starts at row `1`.
- Values are separated by spaces, so a single value cannot contain a space.
- The current database file is stored in `env/dbname`; switch it with `chdb <path>`.

## Commands

| Command | Description |
| --- | --- |
| `help` | Show help |
| `chdb <path>` | Switch the database file |
| `read` | Print the whole table with IDs |
| `find <value> [in <col\|name\|ALL>] [ci] [num\|str] [substr] [ids\|count]` | Search by value |
| `where <condition> [ci] [num\|str] [ids\|count]` | Filter by condition |
| `get <row> <col>` | Get a single cell |
| `getcol <col>` | Get a column |
| `getrow <row>` | Get a row |
| `add <element...>` | Add a row |
| `addcol <name>` | Add a column |
| `set <row> <col> <value>` | Change a cell |
| `setrow <row> <value...>` | Replace a row |
| `setcol <col> <value...>` | Replace a column |
| `renamecol <col\|name> <new_name>` | Rename a column |
| `replace <col\|ALL> <old> <new> [ci] [num]` | Replace whole values |
| `sort <col\|ALL> [asc\|desc] [str\|num] [ci] [view]` | Sort rows |
| `unique <col\|ALL> [ci] [num] [count]` | Unique values or rows |
| `delete <row> <col>` | Clear a cell |
| `delrow <row>` | Clear a row |
| `delcol <col>` | Delete a column |
| `run <file>` | Execute a script |

The full help with all flags and examples is available via `./bytebox help` (file `env/help.txt`).

## Examples

```bash
# Search all columns case-insensitively
./bytebox find berlin ci

# Filter by a numeric condition
./bytebox where age >= 18 and age < 65 num

# Sort by age in descending order
./bytebox sort 2 desc num

# Unique cities with occurrence counts
./bytebox unique 2 ci count

# Bulk-replace empty cells
./bytebox replace ALL "" n/a
```

### Scripts

Commands can be combined into a file and executed with `run`:

```
# example.bb
if city = berlin ci
    read
elif age > 30 num
    where name contains a
else
    find nobody
end
```

```bash
./bytebox run example.bb
```

## Project structure

```
main.cpp        — source code
env/help.txt    — help text
env/dbname      — path to the current database
database.db     — default database
```

## Limitations

- Values cannot contain spaces.
- Numeric comparisons work with integers.
- No locking or concurrent access — the project is intended for local use.
