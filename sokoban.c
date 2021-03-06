#include <stdio.h>
#include <stdlib.h>
#include <termio.h>

char name[10]; //이름저장소
int u=0;//언두 배열 
int ucnt=5;//언두 
int mvcnt = 0; //움직임카운트
int n = 0; //스테이지 값
char map[5][30][30] = {0,};//맵
char Undo[7][5][30][30]={0}; //Undo[0]은 read했을때 모습유지, 나머지 undo 명령어를 위한 공
int x, y; //창고지기의 위치
int score[5][6] = {0};	// 랭킹 - 스테이지, 순위별 이동횟수 저장소
char names[5][6][10];	// 랭킹 - 각 스테이지/순위별 플레이어 이름 저장소
int temps, tempn;   // 랭킹 - 스코어랑 이름 저장소템프
char Floor[5][30][30] = {0}; //O위치 확인용 
int getch();
void map_open();// 박성준, 염한울
void inputname();// 박성준, 염한울
void check();// 염한울, 김희환
void pos_storage();// 염한울, 강재일
void map_print();// 박성준, 김희환
void undo();// 강재일,박성준
void saveundo();// 강재일, 박성준
void check_clear();// 염한울, 박성준
void inputkey(char ch);// 염한울,박성준,강재일,김희환
void save();//염한울, 박성준
void fileload();//염한울, 박성준
void man();// 염한울, 강재일
void con();// 박성준,염한울
void ranking_input();// 김희환
void printing();//염한울, 박성준
void ranking_read();// 김희환,박성준




int main(void)//박성준,김희환, 
{
	char a;
	for (int x=0; x<5; x++)
	{
		for (int y=0; y<5; y++)
			names[x][y][0] = '=';
	}
	printf("Start...\n");
	inputname();
	system("clear");
	map_open();
	pos_storage();
	ranking_read();
	while(1){
	system("clear");
	printf("Hello ");
	for(int i=0;i<10;i++){
		printf("%c",name[i]);
	}
	printf("\n");
	printf("count : %d undo : %d\n",mvcnt, ucnt);
	check_clear();
	con();
	check();
	map_print();
	a=getch();
	inputkey(a);
	}
}

int getch(void)
{
	int ch;

	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;

	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;

	tcsetattr(0, TCSAFLUSH, &buf);

	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);

	return ch;
}

   void map_open() //맵 입력
{
    int cnt1=0;
	int cnt2=0; 
   FILE*fp;

   fp = fopen("map.txt","r");

   char t=0;

   for(int z=0; z<5; z++){

    for(int y=0; y<30; y++){

            for(int x=0; x<30; x++){
                fscanf(fp,"%c",&t);
		if(t=='$')
			cnt1++;
		    
		if(t=='O')
			cnt2++;
		    
                if(t=='\n')
                	break;
		    
                if(t=='2'||t=='3'||t=='4'||t=='5'||t=='e')
                	break;
               
                else
                    	map[z][x][y]=t;
            }
            if(t=='2'||t=='3'||t=='4'||t=='5'||t=='e')
                break;
                
	     
        }
	   if(cnt1 != cnt2){
		   printf("맵이 잘못되었습니다.");
		   exit(0);
            }
	    cnt1=0;
	    cnt2=0;

    }
	   fclose(fp);
   }

void inputname() //이름입력
{
   printf("Input name : ");
   for (int i = 0; i < 11; i++){
      name[i] = ' ';
   }
   scanf("%s", name);
}

void check() //창고지기의 위치
{
	int i, j;
	for(i=0; i<30; i++){
		for(j=0; j<30; j++){
			if(map[n][i][j]=='@'){
				x = i;
				y = j;
		}
	}
}
}
	
void pos_storage() //보관장소 좌표
{
    for(int n=0; n<5; n++){
        for(int x=0; x<30; x++){
            for(int y=0; y<30; y++){
                if (map[n][x][y]=='O')
                    Floor[n][x][y] = 1;

            }
        }
    }
}
	
void map_print() //맵출력
{
	
	printf("\n");
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			printf("%c",map[n][j][i]);
		}
		printf("\n");
	}
}

void undo() // 'u' 명령어 함수
	{
     	ucnt--;
	u--;
     	if( u >= 0 ){
     	for(int i=1;i<30;i++){
          for(int j=0;j<30;j++){
          map[n][i][j] = Undo[u][n][i][j];
     	     }
     	}
     	}
     	else{
     	u = 4;
     	for(int i=1;i<30;i++){
        for(int j=0;j<30;j++){
        map[n][i][j] = Undo[u][n][i][j];
        }
   }
   }

	}

void saveundo() // saveundo 함수
{
	for(int i =1;i<30;i++){
      for(int j = 0; j<30; j++){
      		
      		Undo[u][n][i][j] = map[n][i][j];
      	}
      }
	u++;
	if(u==5){
		u=0;
	}
}

void check_clear() // 스테이지 확인 함수
{
    int check_cnt=0;
    for(x=0; x<30; x++){
        for(y=0; y<30; y++){
            if( Floor[n][x][y] == 1 && map[n][x][y] != '$'){
                check_cnt++;
                }
            }
        }
	if (check_cnt == 0){
		score[n][5]= mvcnt;
		ranking_input();
                    n++;
		mvcnt=0;
		ucnt=5;
			}
    }
	
void inputkey(char ch) // 방향키를 받는 함수
{
	check();


    switch (ch){ 
    case 'k': // 위
	mvcnt++;
        saveundo();
      if (map[n][x][y-1] == 36) // $
      {
         if(map[n][x][y-2] == 46 || map[n][x][y-2] == 79)
            // 상자 옆이 빈공간
         {
          if(Floor[n][x][y])
             map[n][x][y] = 79;
          else
             map[n][x][y] = 46;
          map[n][x][y-2] = 36;
          map[n][x][y-1] = 64;
          break;
         }
         else
            break;

      }

      else if (map[n][x][y-1] == 46) // sp(빈 공간)
      {
         if(Floor[n][x][y]) // 1일 경우 O바닥 // 0일 경우 맨바닥
            map[n][x][y] = 79;
         else
            map[n][x][y] = 46;

         map[n][x][y-1] = 64;
         break;
      }
      else if (map[n][x][y-1] == 79)
      {
         if(Floor[n][x][y])
            map[n][x][y] = 79;
         else
            map[n][x][y] = 46;

         map[n][x][y-1] = 64;
         break;

      }
      else
         break;


   case 'l': // 오른쪽
	mvcnt++;
        saveundo();
      if (map[n][x+1][y] == 36) // $
      {
         if(map[n][x+2][y] == 46 || map[n][x+2][y] == 79)
            // 상자 옆이 빈공간
         {
          if(Floor[n][x][y])
             map[n][x][y] = 79;
          else
             map[n][x][y] = 46;
          map[n][x+2][y] = 36;
          map[n][x+1][y] = 64;
          break;
         }
         else

            break;
      }

      else if (map[n][x+1][y] == 46) // sp(빈 공간)
      {
         if(Floor[n][x][y]) // 1일 경우 O바닥 // 0일 경우 맨바닥
            map[n][x][y] = 79;
         else
            map[n][x][y] = 46;

         map[n][x+1][y] = 64;
         break;
      }
      else if (map[n][x+1][y] == 79)
      {
         if(Floor[n][x][y])
            map[n][x][y] = 79;
         else
            map[n][x][y] = 46;

         map[n][x+1][y] = 64;

         break;
      }
      else
         break;

   case 'h': // 왼쪽  /
	mvcnt++; 
        saveundo();
      if (map[n][x-1][y] == 36) // $
      {
         if(map[n][x-2][y] == 46 || map[n][x-2][y] == 79)
            // 상자 옆이 빈공간
         {
          if(Floor[n][x][y])
             map[n][x][y] = 79;
          else
             map[n][x][y] = 46;
          map[n][x-2][y] = 36;
          map[n][x-1][y] = 64;
          break;
         }
         else

            break;
      }

      else if (map[n][x-1][y] == 46) // sp(빈 공간)
      {
         if(Floor[n][x][y]) // 1일 경우 O바닥 // 0일 경우 맨바닥
            map[n][x][y] = 79;
         else
            map[n][x][y] = 46;

         map[n][x-1][y] = 64;
         break;
      }
      else if (map[n][x-1][y] == 79)
      {
         if(Floor[n][x][y])
            map[n][x][y] = 79;
         else
            map[n][x][y] = 46;

         map[n][x-1][y] = 64;

         break;
      }
	else
         break;
    
   case 'j'://아래
        mvcnt++;
        saveundo();
      if (map[n][x][y+1] == 36) // $
      {
         if(map[n][x][y+2] == 46 || map[n][x][y+2] == 79)
            // 상자 옆이 빈공간
         {
          if(Floor[n][x][y])

             map[n][x][y] = 79;
          else
             map[n][x][y] = 46;
          map[n][x][y+2] = 36;
          map[n][x][y+1] = 64;
          break;
         }
         else
            break;
      }

      else if (map[n][x][y+1] == 46) // sp(빈 공간)

      {
         if(Floor[n][x][y]) // 1일 경우 O바닥 // 0일 경우 맨바닥
            map[n][x][y] = 79;
         else
            map[n][x][y] = 46;

         map[n][x][y+1] = 64;
         break;
      }
      else if (map[n][x][y+1] == 79)
      {
         if(Floor[n][x][y])
            map[n][x][y] = 79;
         else
            map[n][x][y] = 46;

         map[n][x][y+1] = 64;
         break;
      }
      else
         break;
    
    
	case 'u':
		if(ucnt==0)
			break;   
		mvcnt++;
		undo();
		break;
	case 'r':
		    map_open();
			break;
   case 'n': 
 	n = 0;
	mvcnt = 0;
	map_open(); 
 	break; 
      
   case 'd': 

	system("clear");

	man();
	
	while(1){

		char tmp;

		tmp=getch();

		if (tmp == 'd')
			break;

	}
	break;
   case 'e':
	save();
	system("clear");
	printf("SEE YOU %s\n",name);
	exit(0);
	break;

   case 'f':
	fileload();
	break;
	    
   case 's':
			save();
			break;
			
   
   case 't':
		    printf("t");
		    char tmp;
		    tmp=getch();
		    if(tmp=='\n'){
			    printing();
		    }
		    else{
			    switch(tmp){
			    	case '1':
					    printf("1");
					    while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    system("clear");
					    printf("map1\n");
					    
						for(int j=0; j<5; j++){
							if(score[0][j]==999999) break;
   						 printf("%d  ", score[0][j]);
    						for(int k=0; k<10; k++){
      						  printf("%c", names[0][j][k]);
						}
							printf("\n");
							
					}
					   	 while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    break;
					    //1단계랭킹
				case '2':
					    printf("2");
					    while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    system("clear");
					    printf("map2\n");
						for(int j=0; j<5; j++){
							if(score[1][j]==999999) break;
   						 printf("%d  ", score[1][j]);
    						for(int k=0; k<10; k++){
      						  printf("%c", names[1][j][k]);
						}
							printf("\n");
					}
					    while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    break;
					    //2단계랭킹	    
				case '3':
					    printf("1");
					    while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    system("clear");
					    printf("map3\n");
						for(int j=0; j<5; j++){
							if(score[2][j]==999999) break;
   						 printf("%d  ", score[2][j]);
    						for(int k=0; k<10; k++){
      						  printf("%c", names[2][j][k]);
						}
							printf("\n");
					}
					    while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    break;
					    //3단계랭킹
				case '4':
					    printf("1");
					    while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    system("clear");
					    printf("map4\n");
						for(int j=0; j<5; j++){
							if(score[3][j]==999999) break;
   						 printf("%d  ", score[3][j]);
    						for(int k=0; k<10; k++){
      						  printf("%c", names[3][j][k]);
						}
							printf("\n");
					}
					    while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    break;
					    //4단계랭킹
				case '5':
					    printf("1");
					    while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    printf("map5\n");
					    system("clear");
						for(int j=0; j<5; j++){
							if(score[4][j]==999999) break;
   						 printf("%d  ", score[4][j]);
    						for(int k=0; k<10; k++){
      						  printf("%c", names[4][j][k]);
						}
							printf("\n");
					}
					    while(1){
						    char tmpp;
						    tmpp=getch();
						    if(tmpp=='\n')
							    break;
					    }
					    break;
					    //5단계랭킹	    
			    }
		    }
	
	break;
	}
   
}
   void save() // 's'명령어 , 시간과 맵상황을 저장해서 sokoban.txt 파일에 저장, 뭐 더 save 할게 있으면 추가하고
{
	
	FILE *save;
	save = fopen("sokoban.txt", "w");

	fprintf(save, "%s\n", name);	
	fprintf(save, "%d\n", mvcnt);	
	fprintf(save, "%d\n", ucnt);	
	fprintf(save, "%d\n", n);

	for (int x = 0; x < 30; x++){  //맵 저장	
		for(int y=0; y<30; y++)	{
			if(map[n][x][y]==0){
				fprintf(save, "%c", '\n');
				break;
			}
			fprintf(save, "%c", map[n][x][y]);
			
		}
		
	}

	fclose(save);
}

void fileload() // save시점에서부터 이어서 게임하게 하는 함수
{
	FILE *load;
	load = fopen("sokoban.txt", "r");
	char a;
	int x=0, y=0;

	fscanf(load, "%s\n", name); 
	fscanf(load, "%d\n%d\n%d\n", &mvcnt, &ucnt, &n);

	for(int x=0; x<30; x++){
		for(int y=0; y<30; y++){
			fscanf(load, "%c", &a);
			if (a == '\n')
				break;
			else 
				map[n][x][y] = a;
		}
	}	
	fclose(load);
}

void man() //d누르면 실행될 수 있도록 만들어야함//
{
   printf("창고지기 조정 : 왼쪽-h, 아래-j, 위-k, 오른쪽-l\n");
   printf("가장 최근의 이동기록 취소(5번까지 사용가능 - u\n");
   printf("현재 맵을 처음부터 다시시작 - r\n");
   printf("첫번째 맵부터 다시시작 - n\n");
   printf("게임 종료(자동저장) - e\n");
   printf("현재 상태 파일에 저장 - s\n");
   printf("save 지점부터 이어서 게임진행 - f\n");
   printf("게임 순위 출력 - t\n");
}

void con()//단계별 완료시 축하메시지
{
   if(n>=5){
   printf("Congratulation");
   exit(0);
   }
}

void ranking_input()  //랭킹입력함수
{
	FILE*ofp;
    	char rdata;
    	ofp = fopen("ranking.txt", "w");
	
	for(int i=0;i<5;i++){
		for(int j=0;j<6;j++){
			if(names[i][j][0]=='=') score[i][j]=999999;
		}
	}
	for (int i=0; i<=4; i++)
	{
		for (int z=0; z<10; z++){
			names[i][5][z] = name[z];
		}
            	for (int m=0; m<=5; m++)
		{
			for (int j=5; j>0; j--)
			{
					if (score[i][j] < score[i][j-1]||score[i][j-1]==0)    //이동횟수 비교
					{
						temps = score[i][j];
						score[i][j] = score[i][j-1];
						score[i][j-1] = temps;
						for (int k=0; k<=9; k++)
						{
							tempn = names[i][j][k];
							names[i][j][k] = names[i][j-1][k];
							names[i][j-1][k] = tempn;
						}
					}
			}
		}
		for (int j=0; j<=4; j++)
		{
			
			fprintf(ofp, "%s", names[i][j]);
			fprintf(ofp, " \n%d\n", score[i][j]);
		}
	}
	fclose(ofp);
}

void ranking_read(){  // ranking.txt에서 랭킹을 읽어오는 함수
    FILE*rifp;
    rifp = fopen("ranking.txt","r");
    for (int i=0; i<5; i++)
    {
        for (int j=0; j<5; j++)
        {
            fscanf(rifp,"%s\n",names[i][j]);
            fscanf(rifp,"%d\n",&score[i][j]);
        }
    }
    fclose(rifp);
}

void printing(){  // 맵 전체 랭킹을 출력하는 함수
    system("clear");
    for(int i=0; i<5; i++){
        printf("map%d\n", i+1);
        for(int j=0; j<5; j++){
		if(score[i][j]==999999) break;
            printf("%d  ", score[i][j]);
            for(int k=0; k<10; k++){
                printf("%c", names[i][j][k]);
	    }
		printf("\n");
        }
    }
	int c;
	while(1){
		c=getch();
		if(c=='\n'){
			break;
		}
	}
}
