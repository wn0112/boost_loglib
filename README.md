# boost_loglib
A pythonic log library based on boost::log.

# Intruduction
- Sync/Async
```c++
logger.setAsync(true); // default: false
```
- File rotate
```
logger.setRotateFileCount(10);
logger.setRotateFileSize(100); // MB
```
- Message format
```C++
logger.debug("start to run ...");
logger.debug("Hello %s, you are the %dth vistor.", "Andy", 100);
```
output:
```
> 2021-10-09 19:41:14 [0x000057c8] [debug] start to run ...
> 2021-10-09 19:41:14 [0x000057c8] [debug] Hello Andy, you are the 100th vistor.
```
# Example
```c++
#include <iostream>
#include "wlog.h"

int main()
{
   WLogger logger("log.log", LOG_LVL::trace, 10, 100); // level: trace; file count: 10; file size: 100MB
   logger.init();
   
   logger.debug("start to run ...");
   logger.debug("Hello %s, you are the %dth vistor.", "Andy", 100);
}
```
# Requirements:
* _boost_1_75_0_ or higher
# OS
* Windows

# Compiler
* VS2019
