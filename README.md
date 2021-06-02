# Test Driver Generator for C
## Demo
https://user-images.githubusercontent.com/20037035/120407901-f0ed8a80-c388-11eb-89fd-2c7562e13892.mov

실행 요령:

1) gcc main.c gon125.c -o tgen

2) ./tgen 테스트케이스파일 sut파일

** gon125.c, gon125.h가 디렉토리 내에 있어야합니다. 

./run_test.sh를 통해 다시 실행시킬 수도 있고,

./a.out 테스트케이스파일 을 통해 여러 테스트케이스 파일을 테스트 해볼 수 있습니다.

main.c를 보시면 

 make_driver_file
 make_shell_script
 run_shell_script
 print_test_results_report

총 네가지 단계로 tgen 프로그램이 구성되어 있는데, 마지막 단계는 스크립트 실행 시에 결과 출력도 함께 되도록 하였습니다.

** MacOS Big Sur / ARM architecture (Apple M1) 에서 테스트 되었습니다.

