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
int score[5][6] = {{99999},{99999},{99999},{99999},{99999}};	// 랭킹 - 스테이지, 순위별 이동횟수 저장소
int rsc[5] = {0};	// 랭킹 - 여기에 방금 끝낸 게임 이동횟수 들어감
char names[5][6][10];	// 랭킹 - 각 스테이지/순위별 플레이어 이름 저장소
int temps, tempn;   // 랭킹 - 스코어랑 이름 저장소템프
char Floor[5][30][30] = {0}; //O위치 확인용 
int getch();
void map_open();
void map_cnt();
void inputname();
void check();
void pos_storage();
void map_print();
int finish();
void undo();
void saveundo();
void inputkey(char ch);
void save();
void man();
void con();
void ranking_input();

	
int main(void)
{
	char a;
	printf("Start...\n");
	inputname();
	system("clear");
	map_open();
	pos_storage();
	map_cnt();
	while(1){
	printf("Hello %s\n",name);
	printf("count : %d undo : %d\n",mvcnt, ucnt);
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

   FILE*fp;

   fp = fopen("map.txt","r");

   char t=0;

   for(int z=0; z<5; z++){

    for(int y=0; y<30; y++){

            for(int x=0; x<30; x++){
                fscanf(fp,"%c",&t);
                if(t=='\n')
                break;
                if(t=='2'||t=='3'||t=='4'||t=='5'||t=='e'){
                break;
                }
                else
                    map[z][x][y]=t;
            }
            if(t=='2'||t=='3'||t=='4'||t=='5'||t=='e'){
                break;
                }
        }
            }
   fclose(fp);
    }




void map_cnt() //맵박스개수체크
{
   int cnt1 = 0;
   int cnt2 = 0;
   for (int i = 0; i < 5; i++) {
	  for (int j = 0; j < 30; i++) {
		 for (int k = 0; k < 30; k++) {
			if (map[i][j][k] == '$')
			   cnt1++;
			if (map[i][j][k] == 'O')
			   cnt2++;
		 }
	  }
	  if (cnt1 != cnt2) {

	  printf("잘못된 맵입니다.");

	  }
	  cnt1 = 0;
	  cnt2 = 0;
   }

}


void inputname() //이름입력
{
   printf("input name : ");
   scanf("%s",name);
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

/*int check_x() // '@'의 위치(세로축)
{
     int check_x;
     for(int i=1; i< size[n] ; i++){
          for(int j=0 ; j < 30; j++)
          {
               if(map[n][i][j] == '@')
                    check_x = i;
          }}
     return check_x;
}
int check_y() // '@'의 위치(가로축)
{
     int check_y;
     for(int i=1; i< size[n] ; i++){
          for(int  j=0 ; j < 30; j++)
          {
               if(map[n][i][j] == '@')
                    check_y = j;
          }}
     return check_y;
}*/
	
int finish() // 맵이 정상적으로 끝나는지를 확인하는 함수, Floor배열을 통해 제 위치에 '$'가 있는지 확인
{
     int k = 0;
     for(int i=0; i< 30 ; i++){
          for(int j=0;j<30;j++){
               while(Floor[n][i][j] == 1){
                    if(map[n][i][j] == '$')
                         k++;
                         break;
               }}}
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

void saveundo()
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
	
void inputkey(char ch)
{
   /* x = check_x();

    y = check_y();
*/
	check();


    switch (ch){
    case 'k': // 위; 좌표 감소
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


   case 'l': // 아래; 좌표 증가
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

   case 'h': // 위; 좌표 감소
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
    }
    
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
	
	break;
   
   case 't':
	
	break;
}
   
}
   void save() // 's'명령어 , 시간과 맵상황을 저장해서 sokoban.txt 파일에 저장, 뭐 더 save 할게 있으면 추가하고
	{
	FILE *out;
	out = fopen("sokoban.txt","w");
	for(int i=0;i<10;i++)
	fprintf(out,"%c",name[i]);
	fprintf(out,"\n");
	fprintf(out,"%d\n",n);
	int temp = n;
	for(n; n < 5; n++){
     		for(int i=1 ; i<30 ;i ++){
          		for(int j=0 ;j<30 ; j++){
              		fprintf(out,"%c",map[n][i][j]);
                       		  }
               		fprintf(out,"\n");
                     		    }
               		fprintf(out,"\n");}
	n = temp;
	fclose(out);
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
   printf("Congratulation");
   return 0;
}

void ranking_input()  //랭킹입력함수
{
	FILE*ofp;
    	char rdata;
    	ofp = fopen("ranking.txt", "w");
	char map[5] = "!@#$%";
	for (int i=0; i<=4; i++)
	{
		score[i][5] = rsc[i];
		for (int z=0; z<10; z++)
			names[i][5][z] = name[z];
		fprintf(ofp, "%c\n\n",map[i]);
            	for (int m=0; m<=5; m++)
		{
			for (int j=5; j>0; j--)
			{
				if (score[i][j-1] <10)	//위의 점수값이 0일때
				{
					temps = score[i][j];
					score[i][j] = score[i][j-1];
					score[i][j-1] = temps;
					for (int k=0; k<=9; k++)
					{
						tempn = names[i][j][k];
						names[i][j][k] = names[i][j-1][k];
						names[i][j-1][k] =tempn;
					}
				}
				else
				{
					if (score[i][j] < score[i][j-1])    //이동횟수 비교
					{
						temps = score[i][j];
						score[i][j] = score[i][j-1];
						score[i][j-1] = temps;
						for (int k=0; k<=9; k++)
						{
							tempn = names[i][j][k];
							names[i][j][k] = names[i][j+1][k];
							names[i][j-1][k] = tempn;
						}
					}
				}
			}
		}
		for (int j=0; j<=4; j++)
		{
			for (int k=0; k<=9; k++)
				fprintf(ofp, "%c", names[i][j][k]);
			fprintf(ofp, " %d\n\n", score[i][j]);
		}
	}
	fclose(ofp);
}
