[example list]

num | name | input0 |input(fin0)|input(fin3) | elf | source code| mem
---- |---- | ---- | ---- | ----| ----| ----| ----
1 |multiplication | O | O | O | O| O| O
2 |matirx multiplication1 | O | O | O | O| O| O
3 |encryption | O |O | O | O| O| O
4 |matirx multiplication2 | O |O | O| O| O| O
5 |loop unrolling1(iujk) | O | O | O| O| O| O
6 |loop unrolling2(ijuk) | O |O | O| O| O| O
7 |natural merge sort | O |O | O| O| O| O

[kite_result/ERROR(%)]
num | name | input0 |input(fin0)|input(fin3)
---- |---- | ---- | ----  | ---- 
1 |multiplication | 24.6153 | 24.6153|24.6153
2 |matirx multiplication1 | 9.5441 | 8.8649| 8.8649
3 |encryption | 7.5160| 7.5160|7.5160
4 |matirx multiplication2 | 12.0871 | 12.1212 |12.1212
*5 |loop unrolling1(iujk) | 9.5296 | 9.5951 |*10.0486
*6 |loop unrolling2(ijuk) | 7.8851 | 7.9131|*8.2243
7 |natural merge sort | 13.3286 | X |13.4421

fin0, natural merge sort X: "Error: program size goes out of the code segment."
	
![image](https://user-images.githubusercontent.com/76509344/232783197-da4d36bc-cb30-4ebc-9a1c-aab697f9d55a.png)
