# autocomplete
 A command-line text autocomplete tool that uses Redis tries

## Using the tool

###### Batch Mode:
 Batch mode applies the autocomplete tool to a file of prefixes. The input file must be formatted with
 a new line for each prefix, e.g.
 ```
 Prefixone
 Anotherprefix
 Yetanother
 ```
 To call the tool, use the command
 ```
 ./autocomplete -b <-w> <-n NUM> <-d DICT> -f FILE
 ```
 Only the -b and -f FILE flags are required, all others are optional.

 The -b flag indicates that the tool is being used in batch mode.
 The -w flag, if present, enables the display of the first n completions of each prefix in the file.
 The -n flag changes the number of completions shown from its default value, 10, to the value specified (NUM).
 The -d flag allows the setting of a non-standard dictionary, and the DICT file can be specified by e.g. ./dictionary.text
 The -f flag is for the file of prefixes to be declared, specified by e.g. ./prefixes.text

 The output is of the form
 ```
 Lo: ........... 6, [Lo, Long, Longer, Longest, Loop, Looping]
 ```
 for example, with each prefix on its own line. Without the -w flag set, the bracketed words would not be displayed.
