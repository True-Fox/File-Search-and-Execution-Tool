# Unix/Linux File Search and Execution Tool

This tool is designed to simplify the process of searching directories for specific files or file types on Unix/Linux systems. Additionally, it offers the functionality to execute executable files found during the search.

### Usage
```bash
./file_search_tool [OPTIONS]
```

Options:
 - -name PATTERN: Search for files with a specific name pattern.
 - -type TYPE: Specify the type of file to search for (f: regular file, d: directory).
 - -path STRING: Set the path where the search should be performed (default is the current directory).
 - -exec: Execute the found executable files.
 - -h, --help: Display help and exit.

### Examples

- Search for all text files in the current directory:
```bash
./file_search_tool -name "*.txt"
```

- Search for executable files and execute them:
  ```bash
  ./file_search_tool -type f -exec
  ```

### Note
- The tool follows symbolic links by default. Use the `-no-follow-links` option to disable following symbolic links.
- If no options are provided, the tool will search for all files in the current directory.
