#!/bin/bash
UNAME=$(uname -s)
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

echo -e "${YELLOW}Universal tests:${NC}"

#OK
cat -b cat_test.txt > cat_test_results.txt
./s21_cat -b cat_test.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 1: flag -b is successful${NC}"
else echo -e "${RED}Test 1: flag -b failed${NC}" 
fi

#OK
cat -b cat_test.txt cat_test1.txt cat_test2.txt > cat_test_results.txt
./s21_cat -b cat_test.txt cat_test1.txt cat_test2.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 2: flag -b with multiple files is successful${NC}"
else echo -e "${RED}Test 2: flag -b with multiple files failed${NC}" 
fi

#OK
cat -e cat_test.txt > cat_test_results.txt
./s21_cat -e cat_test.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 3: flag -e is successful${NC}"
else echo -e "${RED}Test 3: flag -e failed${NC}" 
fi

#OK
cat -n cat_test.txt > cat_test_results.txt
./s21_cat -n cat_test.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 4: flag -n is successful${NC}"
else echo -e "${RED}Test 4: flag -n failed${NC}"  
fi

#OK
cat -s cat_test.txt > cat_test_results.txt
./s21_cat -s cat_test.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 5: flag -s is successful${NC}"
else echo -e "${RED}Test 5: flag -s failed${NC}"
fi

#OK
cat -t cat_test.txt > cat_test_results.txt
./s21_cat -t cat_test.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 6: flag -t is successful${NC}"
else echo -e "${RED}Test 6: flag -t failed${NC}"
fi

#OK
cat -benst cat_test.txt > cat_test_results.txt
./s21_cat -benst cat_test.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 7: flags -benst is successful${NC}"
else echo -e "${RED}Test 7: flag -benst failed${NC}" 
fi

#OK
cat cat_test.txt > cat_test_results.txt
./s21_cat cat_test.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 8: no flag is successful${NC}"
else echo -e "${RED}Test 8: no flag failed${NC}" 
fi

#OK
cat -v cat_test.txt > cat_test_results.txt
./s21_cat -v cat_test.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 9: flag -v is successful${NC}"
else echo -e "${RED}Test 9: flag -v failed${NC}"  
fi

#OK
cat -benstv cat_test.txt > cat_test_results.txt
./s21_cat -benstv cat_test.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 10: flag -benstv is successful${NC}"
else echo -e "${RED}Test 10: flag -benstv failed${NC}"  
fi

#OK
cat -enstv cat_test.txt cat_test1.txt cat_test2.txt > cat_test_results.txt
./s21_cat -enstv cat_test.txt cat_test1.txt cat_test2.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 11: flag -enstv with multiple files is successful${NC}"
else echo -e "${RED}Test 11: flag -enstv with multiple files failed${NC}"  
fi

#OK
cat -v cat_test3.txt cat_test4.txt > cat_test_results.txt
./s21_cat -v cat_test3.txt cat_test4.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 12: flag -v (char from 0 to 256 and emoji) is successful${NC}"
else echo -e "${RED}Test 12: flag (char from 0 to 256 and emoji) -v failed${NC}"  
fi

#Linux tests
if [ $UNAME =  Linux ]
then echo -e "${YELLOW}Linux tests:${NC}"
cat -T cat_test.txt cat_test1.txt cat_test2.txt > cat_test_results.txt
./s21_cat -T cat_test.txt cat_test1.txt cat_test2.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 13: flag -T with multiple files is successful${NC}"
else echo -e "${RED}Test 13: flag -T with multiple files failed${NC}"  
fi

cat -E cat_test.txt cat_test1.txt cat_test2.txt > cat_test_results.txt
./s21_cat -E cat_test.txt cat_test1.txt cat_test2.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 14: flag -E with multiple files is successful${NC}"
else echo -e "${RED}Test 14: flag -E with multiple files failed${NC}"  
fi

cat -A cat_test.txt cat_test1.txt cat_test2.txt > cat_test_results.txt
./s21_cat -A cat_test.txt cat_test1.txt cat_test2.txt > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 15: flag -A with multiple files is successful${NC}"
else echo -e "${RED}Test 15: flag -A with multiple files failed${NC}"  
fi

cat --number-nonblank cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt > cat_test_results.txt
./s21_cat --number-nonblank cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt  > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 16: flag --number-nonblank with multiple files is successful${NC}"
else echo -e "${RED}Test 16: flag --number-nonblank with multiple files failed${NC}"  
fi

cat --show-ends cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt > cat_test_results.txt
./s21_cat --show-ends cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt  > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 17: flag --show-ends with multiple files is successful${NC}"
else echo -e "${RED}Test 17: flag --show-ends with multiple files failed${NC}"  
fi

cat --number cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt > cat_test_results.txt
./s21_cat --number cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt  > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 18: flag --number with multiple files is successful${NC}"
else echo -e "${RED}Test 18: flag --number with multiple files failed${NC}"  
fi

cat --squeeze-blank cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt > cat_test_results.txt
./s21_cat --squeeze-blank cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt  > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 19: flag --squeeze-blank with multiple files is successful${NC}"
else echo -e "${RED}Test 19: flag --squeeze-blank with multiple files failed${NC}"  
fi

cat --show-nonprinting --show-tabs cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt > cat_test_results.txt
./s21_cat --show-nonprinting --show-tabs cat_test.txt cat_test1.txt cat_test2.txt cat_test3.txt cat_test4.txt  > s21_cat_test_results.txt
diff cat_test_results.txt s21_cat_test_results.txt
if [ $? = 0 ]
then echo -e "${GREEN}Test 20: flag --show-nonprinting --show-tabs with multiple files is successful${NC}"
else echo -e "${RED}Test 20: flag --show-nonprinting --show-tabs with multiple files failed${NC}"  
fi

fi