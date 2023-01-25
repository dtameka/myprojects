#!/bin/bash
UNAME=$(uname -s)
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

echo -e "${YELLOW}Single option tests:${NC}"

# test 1, no option (hidden option -e), single file
grep r grep_test.txt > grep_test_results.txt
./s21_grep r grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 1: no option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 1: no option, single file - s21_grep failed${NC}" 
fi

# test 2, no option (hidden option -e), multiple files
grep exam grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep exam grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 2: no option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 2: no option, multiplr files - s21_grep failed${NC}" 
fi

# test 3, option -e, single file
grep -e e grep_test.txt > grep_test_results.txt
./s21_grep -e e grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 3: -e option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 3: -e option, single file - s21_grep failed${NC}" 
fi

# test 4, option -e, multiple files
grep -e does grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -e does grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 4: -e option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 4: -e option, multiple files - s21_grep failed${NC}"
fi

# test 5, option -e, regex pattern, multiple files
grep -e [a-z] grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -e [a-z] grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 5: -e, regex pattern, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 5: -e, regex pattern, multiple files - s21_grep failed${NC}" 
fi

# test 6, option -i, single file
grep -i E grep_test.txt > grep_test_results.txt
./s21_grep -i E grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 6: -i option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 6: -i option, single file - s21_grep failed${NC}" 
fi

# test 7, option -i, multiple files
grep -i TT grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -i TT grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 7: -i option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 7: -i option, multiple files - s21_grep failed${NC}" 
fi

# test 8, option -v, single file
grep -v E grep_test.txt > grep_test_results.txt
./s21_grep -v E grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 8: -v option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 8: -v option, single file - s21_grep failed${NC}" 
fi

# test 9, option -v, multiple files
grep -v E grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -v E grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 9: -v option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 9: -v option, multiple files - s21_grep failed${NC}" 
fi

# test 10, option -c, single file
grep -c is grep_test.txt > grep_test_results.txt
./s21_grep -c is grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 10: -c option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 10: -c option, single file - s21_grep failed${NC}" 
fi

# test 11, option -c, multiple files
grep -c ex grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -c ex grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 11: -c option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 11: -c option, multiple files - s21_grep failed${NC}" 
fi

# test 12, option -l, single file
grep -l is grep_test.txt > grep_test_results.txt
./s21_grep -l is grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 12: -l option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 12: -l option, single file - s21_grep failed${NC}" 
fi

# test 13, option -l, multiple files
grep -l ex grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -l ex grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 13: -l option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 13: -l option, multiple files - s21_grep failed${NC}" 
fi

# test 14, option -n, single file
grep -n i grep_test.txt > grep_test_results.txt
./s21_grep -n i grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 14: -n option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 14: -n option, single file - s21_grep failed${NC}" 
fi

# test 15, option -n, multiple files
grep -n e grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -n e grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 15: -n option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 15: -n option, multiple files - s21_grep failed${NC}" 
fi

# test 16, option -h, single file
grep -h is grep_test.txt > grep_test_results.txt
./s21_grep -h is grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 16: -h option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 16: -h option, single file - s21_grep failed${NC}" 
fi

# test 17, option -h, multiple files
grep -h ex grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -h ex grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 17: -h option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 17: -h option, multiple files - s21_grep failed${NC}" 
fi

# test 18, option -s, single file
grep -s is grep_test0.txt > grep_test_results.txt
./s21_grep -s is grep_test0.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 18: -s option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 18: -s option, single file - s21_grep failed${NC}" 
fi

# test 19, option -s, multiple files
grep -s is grep_test0.txt grep_testz.txt > grep_test_results.txt
./s21_grep -s is grep_test0.txt grep_testz.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 19: -s option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 19: -s option, multiple files - s21_grep failed${NC}" 
fi

# test 20, option -f, single file
grep -f pattern grep_test.txt > grep_test_results.txt
./s21_grep -f pattern grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 20: -f option, single file -  s21_grep is successful${NC}"
else echo -e "${RED}Test 20: -f option, single file -  s21_grep failed${NC}" 
fi

# test 21, option -f, multiple files - CHECK pattern file for matching!!!
grep -f pattern grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 21: -f option, multiple files -  s21_grep is successful${NC}"
else echo -e "${RED}Test 21: -f option, , multiple files -  s21_grep failed${NC}" 
fi

# test 22, option -o, single file
grep -o am grep_test.txt > grep_test_results.txt
./s21_grep -o am grep_test.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 22: -o option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 22: -o option, single file - s21_grep failed${NC}" 
fi

# test 23, option -o, multiple files
grep -o t grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -o t grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 23: -o option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 23: -o option, multiple files - s21_grep failed${NC}" 
fi

#bonus
if [ $UNAME =  Linux ]
then echo -e "${YELLOW}Linux tests:${NC}"

# test 24, option -on, multiple files
grep -on t grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -on t grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 24: -on option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 24: -on option, multiple files - s21_grep failed${NC}" 
fi

# test 25, option -onh, multiple files
grep -onh t grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -onh t grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 25: -onh option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 25: -onh option, multiple files - s21_grep failed${NC}" 
fi

# test 26, option -onhc, multiple files
grep -onhc t grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -onhc t grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 26: -onhc option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 26: -onhc option, multiple files - s21_grep failed${NC}" 
fi

# test 27, option -onhcl, multiple files
grep -onhcl t grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -onhcl t grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 27: -onhcl option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 27: -onhcl option, multiple files - s21_grep failed${NC}" 
fi

# test 28, option -onhclv, multiple files
grep -onhclv t grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -onhclv t grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 28: -onhclv option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 28: -onhclv option, multiple files - s21_grep failed${NC}" 
fi

# test 29, option -onhclvi, multiple files
grep -onhclvi t grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -onhclvi t grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 29: -onhclvi option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 29: -onhclvi option, multiple files - s21_grep failed${NC}" 
fi

# test 30, option -onhclvie, multiple files
grep -onhclvie t grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -onhclvie t grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 30: -onhclvie option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 30: -onhclvie option, multiple files - s21_grep failed${NC}" 
fi

# test 31, option -onhclvief, multiple files
grep -onhclvie t -f pattern grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -onhclvie t -f pattern grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 31: -onhclvief option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 31: -onhclvief option, multiple files - s21_grep failed${NC}" 
fi

# test 32, option -ov, multiple files
grep -ov t grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -ov t grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 32: -ov option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 32: -ov option, multiple files - s21_grep failed${NC}" 
fi

# test 33, option -incovlhsf, multiple files
grep -incovlhs -f pattern grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -incovlhs -f pattern grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 33: -incovlhsf option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 33: -incovlhsf option, multiple files - s21_grep failed${NC}" 
fi

# test 34, option -ncove, multiple files
grep -ncov -e s grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -ncov -e s grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 34: -ncove option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 34: -ncove option, multiple files - s21_grep failed${NC}" 
fi

# test 35, option -cioe, multiple files
grep -cio -e a grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -cio -e a grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 35: -cioe option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 35: -cioe option, multiple files - s21_grep failed${NC}" 
fi

# test 36, option -chilnosve, multiple files
grep -chilnosv -e i grep_test.txt grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -chilnosv -e i grep_test.txt grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 36: -chilnosve option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 36: -chilnosve option, multiple files - s21_grep failed${NC}" 
fi

# test 37, option -fo, multiple files
grep -f pattern1 -o grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern1 -o grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 37: -fo option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 37: -fo option, multiple files - s21_grep failed${NC}" 
fi

# test 38, option -fo, multiple files
grep -f pattern2 -o grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern2 -o grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 38: -fo option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 38: -fo option, multiple files - s21_grep failed${NC}" 
fi

# test 39, option -fo, single file
grep -f pattern2 -o grep_test1.txt  > grep_test_results.txt
./s21_grep -f pattern2 -o grep_test1.txt  > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 39: -fo option, single file - s21_grep is successful${NC}"
else echo -e "${RED}Test 39: -fo option, single file - s21_grep failed${NC}" 
fi

# test 40, option -fio, multiple files
grep -f pattern2 -io grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern2 -io grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 40: -fio option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 40: -fio option, multiple files - s21_grep failed${NC}" 
fi

# test 41, option -fino, multiple files
grep -f pattern2 -ino grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern2 -ino grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 41: -fino option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 41: -fino option, multiple files - s21_grep failed${NC}" 
fi

# test 42, option -fioh, multiple files
grep -f pattern2 -ioh grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern2 -ioh grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 42: -fioh option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 42: -fioh option, multiple files - s21_grep failed${NC}" 
fi

# test 43, option -fios, multiple files
grep -f pattern2 -ios grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern2 -ios grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 43: -fios option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 43: -fios option, multiple files - s21_grep failed${NC}" 
fi

# test 44, option -finoh, multiple files
grep -f pattern2 -inoh grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern2 -inoh grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 44: -finoh option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 44: -finoh option, multiple files - s21_grep failed${NC}" 
fi

# test 45, option -fiohs, multiple files
grep -f pattern2 -iohs grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern2 -iohs grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 45: -fiohs option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 45: -fiohs option, multiple files - s21_grep failed${NC}" 
fi

# test 46, option -fnios, multiple files
grep -f pattern2 -inos grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern2 -inos grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 46: -finos option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 46: -finos option, multiple files - s21_grep failed${NC}" 
fi

# test 47, option -fniohs, multiple files
grep -f pattern1 -inohs grep_test1.txt grep_test2.txt > grep_test_results.txt
./s21_grep -f pattern1 -inohs grep_test1.txt grep_test2.txt > s21_grep_test_results.txt
diff grep_test_results.txt s21_grep_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 47: -finohs option, multiple files - s21_grep is successful${NC}"
else echo -e "${RED}Test 47: -finohs option, multiple files - s21_grep failed${NC}" 
fi

fi