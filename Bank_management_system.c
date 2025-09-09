#include <stdio.h>
#include <string.h>

void create_account();
void deposite_money();
void withdraw_money();
void check_balance();

const char *ACCOUNT_NAME = "account.dat";

typedef struct bank
{
	char name[50];
	int acc;
	float balance;
} account;

int main()
{
	int ch;

	while (1)
	{
		printf("\n***Welcome to Bank Management***\n");
		printf("__**__Menu__**__\n1.Check Account Balance\n2.Deposit Money\n3.Withdraw Money\n4.Create New Account\n5.Exit\n");
		printf("Enter your choice :\n");
		scanf("%d", &ch);
		if (ch == 5)
		{
			printf("Exiting...\n");
			printf("Thank You Visiting....!");
			break;
		}

		switch (ch)
		{
		case 1:
			check_balance();
			break;
		case 2:
			deposite_money();
			break;
		case 3:
			withdraw_money();
			break;
		case 4:
			create_account();
		default:
			printf("Invalid Choice\n");
		}
	}
	return 0;
}
void create_account()
{
	account acc;
	FILE *file = fopen(ACCOUNT_NAME, "ab+");
	if (file == NULL)
	{
		printf("Unable to open file!\n");
		return;
	}

	getchar(); // to consume the newline character left by previous scanf
	printf("\nEnter your name:\n");
	fgets(acc.name, sizeof(acc.name), stdin);
	int ind = strcspn(acc.name, "\n");
	acc.name[ind] = '\0';

	printf("Enter your account number : \n");
	scanf("%d", &acc.acc);
	acc.balance = 0;

	fwrite(&acc, sizeof(acc), 1, file);
	fclose(file);
	printf("Account created successfully..!\n");
}
void deposite_money()
{
	FILE *file = fopen(ACCOUNT_NAME, "rb+");
	if (file == NULL)
	{
		printf("Unable to open file!\n");
		return;
	}
	int acc;	
	account acc_read;
	printf("Enter the account number: \n");
	scanf("%d", &acc);
	while (fread(&acc_read, sizeof(acc_read), 1, file))
	{
		if (acc_read.acc == acc)
		{
			float amount;
			printf("Enter the amount to deposit: \n");
			scanf("%f", &amount);
			acc_read.balance += amount;

			fseek(file, -sizeof(acc_read), SEEK_CUR);
			fwrite(&acc_read, sizeof(acc_read), 1, file);
			fclose(file);
			printf("Amount deposited successfully. New Balance is %.2f\n", acc_read.balance);
			return;
		}
	}
}
void withdraw_money()
{
	FILE *file = fopen(ACCOUNT_NAME, "rb+");
	if (file == NULL)
	{
		printf("Unable to open file!\n");
		return;
	}
	int acc;
	account acc_read;
	printf("Enter the account number: \n");
	scanf("%d", &acc);
	while (fread(&acc_read, sizeof(acc_read), 1, file))
	{
		if (acc_read.acc == acc)
		{
			float amount;
			printf("Enter the amount to withdraw: \n");
			scanf("%f", &amount);
			if (amount > acc_read.balance)
			{
				printf("Insufficient balance. Current Balance is %.2f\n", acc_read.balance);
				fclose(file);
				return;
			}
			acc_read.balance -= amount;

			fseek(file, -sizeof(acc_read), SEEK_CUR);
			fwrite(&acc_read, sizeof(acc_read), 1, file);
			fclose(file);
			printf("Amount withdrawn successfully. New Balance is %.2f\n", acc_read.balance);
			return;
		}
	}
	printf("Account number %d was not found.\n", acc);
	fclose(file);
}
void check_balance()
{
	FILE *file = fopen(ACCOUNT_NAME, "rb");
	if (file == NULL)
	{
		printf("Unable to open file!\n");
		return;
	}
	int acc;
	account acc_read;
	printf("Enter the account number: \n");
	scanf("%d", &acc);
	while (fread(&acc_read, sizeof(acc_read), 1, file))
	{
		if (acc_read.acc == acc)
		{
			printf("Your Current Balance is %.2f\n", acc_read.balance);
			fclose(file);
			return;
		}
	}
	printf("Account number %d was not found.\n", acc);
}