# TSE
## Chandan Aggarwal (caggarwal8)

This Tiny Search Engine, when given a seed URL in the command line, recursively crawls and extracts all embedded links on the page until a certain depth (i.e. distance from the seed URL), indexing each word found on each page and writing it to a file provided in the command line. The program then accepts queries from the command line that can include && and || operators, and, if the user has inputted a valid entry, it then returns a list of URLs based on a ranking of how many times the given query appears on the page.

