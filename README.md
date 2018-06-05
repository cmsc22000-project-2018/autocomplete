# autocomplete
 A command-line text autocomplete tool that uses Redis-stored trie data structures to store its dictionary.

 At a high level, there are two modes: interactive mode and batch mode.

 In Interactive mode, the user can interactively type text and autocomplete individual words as you go.

 In batch mode, the user feeds the program a file of prefixes and the program returns a set number of possible completions,
 and the total number of possible competions.

## Using the tool

### Autocomplete Shell
 After running interactive mode, you will enter the interactive mode shell. It looks like this:
 ```
 AUTOCOMPLETE Instructions: interactive <flags> || batch <flags> || exit
 >>>:
 ```
 From here, you can re-enter interactive mode, or use batch mode.

 Known bug: Commands sporadically don't work. If that happens, just enter the command again!

 For a link to this README, run ./autocomplete with the 'help' or '-h' flag.

### Batch Mode
 Batch mode applies the autocomplete tool to a file of prefixes. The input file must be formatted with
 a new line for each prefix, e.g.
 ```
 Prefixone
 Anotherprefix
 Yetanother
 ```
 To call the tool, use the command
 ```
 ./autocomplete -b <-w> <-n NUM> <-d DICT> -f FILE <-o OUT>
 ```
 Only the -b and -f FILE flags are required, all others are optional.

 The -b flag indicates that the tool is being used in batch mode.

 The -w flag, if present, enables the display of the first n completions of each prefix in the file.

 The -n flag changes the number of completions shown from its default value, 10, to the value specified (NUM).

 The -d flag allows the setting of a non-standard dictionary, and the DICT file (formatted just like the input) can be specified by e.g. ./dictionary.text

 The -f flag is for the file of prefixes to be declared, specified by e.g. ./prefixes.text

 The -o flag allows for the output to be placed in a file rather than printed to the terminal, specified by e.g. ./out.txt

 The output is of the form
 ```
 Lo: ........... 6, [Lo, Long, Longer, Longest, Loop, Looping]
 ```
 for example, with each prefix on its own line. Without the -w flag set, the bracketed words would not be displayed.

### Interactive Mode

 Interactive mode opens a terminal-based text editor written using the ncurses library. Once in the editor, you can type as normal and then press "tab" to autocomplete the current word.

 From the terminal, you can enter interactive mode with the following command line prompts:
 ```
 ./autocomplete
 ./autocomplete -i <flags>
 ```
 From the autocomplete shell, you can enter interactive mode with the following command line prompts:
 ```
 ./autocomplete
 ./autocomplete interactive <flags>
 ```
  Interactive mode has the following flags:
  ```
  -d <file path>
  -n <natural number>
  ```
  -d specifies a local dictionary file to use, and -n specifies the number of completions to display.

  Example call:
  ```
  ./autocomplete -d src/test_dict.txt -n 5
  ```

 Text editor interface:
* When you start the editor, a prompt with instructions will appear. Press enter to start typing.
* While in the editor, the following keyboard commands are valid:
    * Typing
    * Deleting anything but new lines
    * ` saves the file to the current directory with either a custom name or the file "autocomplete_save.txt"
    * ~ exits interactive mode into the autocomplete shell
    * Pressing tab displays n (default: 10) autocomplete options for the most recently typed word. If tab is pressed when only one completion exists, that completion will be filled automatically. Pressing "enter" on an option clears the options and prints the selected option to the screen, autocompleting the word.
