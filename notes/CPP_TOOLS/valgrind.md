# Common Valgrind Commands & Scenarios 
## Basic Usage
* Command runs your executable binary program, providing a basic memory check
```
valgrind ./exec_program 
```

## Memory Leak Detection 
* Command enables details leak detection, showing where memory was allocated and not freed.
```
valgrind --track-origins=yes ./exec_program
```

## Tracking Origins of Uninitialized Values
* Command to identify where uninitialized values come from providing more insight into potential issues. 
```
valgrind --track-origins=yes ./exec_program 
```

## Supressing Warnings
* Command will generate a `supressions.txt` file to help you identify which warnings to ignore
* This command helps if you encounter known issues or third-party library leaks you wish to supress. 
```
valgrind --suppressions=suppressions.txt ./exec_program
```

## Generating a Detailed Report
* Command will generate the Valgrind report to a specified log file for easier analysis
```
valgrind --log-file=valgrind.log ./exec_program
```

## Multi-threaded Applications
* Command helps you to analyze multi-threaded programs 
```
valgrind --track-fds=yes --tool=helgrind ./multi-thread-programs
```


