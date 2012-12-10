#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Enter Number: ");
    long long num = GetLongLong();
    long long temp = num;
    
    int position = 0;
    int total = 0;
    int firstDigit;
    int firstTwoDigits;
    
    while(temp > 0)
    {
        int digit = temp%10;
        if(position%2 == 1)
        {
            total += (2*digit)%10 + (2*digit)/10;
        }
        else
        {
            total += digit;
        }
        
        if(temp >= 10 && temp < 100)
            firstTwoDigits = temp;
        if(temp < 10 && temp != 0)
            firstDigit = temp;
            
        temp = temp/10;
        position++;
    }
    
    if(total%10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }
    
    if(position == 15 && (firstTwoDigits == 34 || firstTwoDigits == 37))
    {
        printf("AMEX\n");
        return 0;
    }
    
    if(position == 16 && (firstTwoDigits >= 51 && firstTwoDigits <= 55))
    {
        printf("MASTERCARD\n");
        return 0;
    }
    
    if((position == 16 || position == 13) && firstDigit == 4)
    {
        printf("VISA\n");
        return 0;
    }
    
    printf("INVALID\n");
    return 0;
}
