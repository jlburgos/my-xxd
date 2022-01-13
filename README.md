# Simple tool to mimic the 'xxd -i' command output

## Example output:
```
$ make

$ ./bin/my-xxd
./<prog> [src] [target]

$ cat test.txt
A sample file.

$ ./bin/my-xxd test.txt test.txt.hpp
Generated out file: "test.txt.hpp"

$ cat test.txt.hpp
#ifndef __TEST_TXT_HPP
#define __TEST_TXT_HPP

#include <cstddef>

unsigned char TEST_TXT[] = {
        0x41, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x66, 0x69, 0x6c,
        0x65, 0x2e,
};

std::size_t TEST_TXT_LEN = 16;

#endif /* __TEST_TXT_HPP */
```