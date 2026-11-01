Bytebox help:

<> - Required argument
[] - Optional elements
|  - Or

The first row is the header. Row IDs start at 1; the header is row 0.
Values are separated by spaces, so a single value cannot contain a space.

help - show help menu

chdb <path> - switch the database file
    The path is saved to env/dbname. The new database is created by the next
    command that changes data. Relative paths are resolved from the current directory.

read - print the whole database with IDs and aligned columns

find/
    find <value> [in <column|name|ALL>] [ci] [num|str] [substr] [ids|count]
        Search data rows and print the matching rows with IDs.
        in <column>  search only this column (default: ALL)
        ci           ignore case
        num          compare numbers numerically; non-numeric values are
                     compared as text for = and !=
        str          compare as text (default)
        substr       match a substring instead of the whole value
        ids          print only matching row IDs
        count        print only the number of matching rows
        The database is not changed.

        Examples:
            find alice
            find berlin in city ci
            find ann in name substr
            find 42 in age num count

where/
    where <condition> [ci] [num|str] [ids|count] - filter rows
        A condition is a boolean expression:
            <column|name|ALL> <operator> <value>
            <column|name|ALL> empty
            <column|name|ALL> notempty
            not <condition>
            ( <condition> )
            <condition> and <condition>
            <condition> or <condition>

        Operators: = == != < <= > >= contains starts ends
        ALL matches when at least one cell satisfies the comparison.
        Use not before a comparison to require that no cell matches.
        By default matching rows are printed with IDs.
        ids / count print only IDs or the number of matching rows.
        ci ignores case; num compares numbers numerically; str compares as text.
        With num, non-numeric values are text for = and !=; < <= > >= are false.
        num cannot be used with contains/starts/ends.
        The database is not changed.

        Examples:
            where age > 30 num
            where city = berlin ci
            where name contains a or name contains b
            where age >= 18 and age < 65 num
            where city empty
            where not city = berlin ci
            where name starts a ids

get/
    get <row> <column>    - print one element
    getcol <column>       - print a column (header excluded)
    getrow <row>          - print a row (row 0 prints the header)

add/
    add <element...>      - add a new row at the end
    addcol <column_name>  - add a column name to the header

set/
    set <row> <column> <value> - replace one element
    setrow <row> <value...>    - replace a row; value count must equal row width
    setcol <column> <value...> - replace a column; one value per row with this column

rename/
    renamecol <column|name> <new_name> - rename a column in the header
        The column can be given by index or by its current name.
        Duplicate column names are rejected.

replace/
    replace <column|ALL> <old> <new> [ci] [num] - replace whole values
        <column>  column index (the header is never changed)
        ALL       replace in every cell of data rows
        ci        ignore case when comparing text
        num       compare values as integers; non-numeric values count as text
        str       compare values as text (default)

        Only whole values are matched, not substrings. Use "" as <old> to
        replace empty cells. The database is written only if something changed.

        Examples:
            replace 0 alice ALICE
            replace 1 berlin Berlin ci
            replace 2 7 007 num
            replace ALL "" n/a

sort/
    sort <column|ALL> [asc|desc] [str|num] [ci] [view] - sort data rows
        <column>  column index to sort by
        ALL       sort by all columns, left to right
        asc       ascending order (default)
        desc      descending order
        str       compare values as text (default)
        num       compare values as integers; non-numeric values go last
        ci        ignore case when comparing text
        view      print the sorted result without saving the database

        The header stays in row 0. Missing and empty cells always go last.

        Examples:
            sort 0              sort by the first column as text, ascending
            sort 2 desc num     sort by the third column numerically, descending
            sort ALL ci view    sort by all columns, ignore case, print only

unique/
    unique <column|ALL> [ci] [num] [count] - print unique values or rows
        <column>  print unique values of one column
        ALL       print unique rows, compared left to right
        ci        ignore case when comparing text
        num       compare values as integers; non-numeric values count as text
        str       compare values as text (default)
        count     print the number of occurrences after each value

        Values are printed in order of first appearance. The database is not changed.

        Examples:
            unique 0            unique values of the first column
            unique 1 ci count   unique values ignoring case, with counts
            unique ALL num      unique rows, with numeric comparison

delete/
    delete <row> <column> - clear one element
    delrow <row>          - clear all values in a row (the row and its ID stay)
    delcol <column>       - delete a column from the header and all rows

run/
    run <file> - execute a script
        Commands are regular commands, one per line. Blank lines and lines
        starting with # are ignored. if/elif check whether at least one data
        row matches the condition. Close a block with end; blocks can be nested.
            if <condition>
                <commands...>
            elif <condition>
                <commands...>
            else
                <commands...>
            end

        Conditions use the same syntax and ci/num/str flags as where.
        Changes are saved after each changing command.

        Example:
            if city = berlin ci
                read
            elif age > 30 num
                where name contains a
            else
                find nobody
            end
