#include <cs50.h>
#include <stdio.h>

int main(void)
{
printf("Height: ");
int height = GetInt();

while(height < 0 || height > 23)
{
    printf("Invalid height, please reenter: ");
    height = GetInt();
}

    for(int i = 1; i <= height; i++)
    { 
      for(int j = 1; j <= height - i; j++)
      {
        printf(" ");
      }
      
      for(int k = 1; k <= i; k++)
      {
        printf("#");
      }
      
      printf(" ");
      printf(" ");
      
      for(int k = 1; k <= i; k++)
      {
        printf("#");
      }
      
      printf("\n");
    }
    
    return 0;
}
