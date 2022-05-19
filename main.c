#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Node
{
    int data[257];  //nilai minterms
    int bin[26];    //minterms dalam binary
    int sumOne;     //jumlah angka 1 dalam binary sebuah minterms
    int isimplicant;    //implicant atau tidak
    char abc[26];   //variabel huruf dari minterms
    struct Node* next;
};

//Global Variables
struct Node *primeNode;
struct Node *head;
struct Node *impNode;
struct Node *save;
struct Node *last;
struct Node *new;
int var;
int min;
int number=1;
int columns=2;
int check=1;
int maxMinterms;
int tab[100][100];
int counter=0;
int essential[1000];
int t=0;
int no=0;
int minterms[1000];
char a[26];       //

//
void inputData(){
    int i,j,k,x;
    struct Node* temp;
    printf("\nMasukkan jumlah variabel : ");       //no. of variables and minterms are recorded
    scanf("%d",&var);
    while(var <= 0){
        printf("Input tidak valid\n");
        printf("\nMasukkan jumlah variabel : ");
        scanf("%d",&var);
    }
    printf("\nMasukkan jumlah minterms: ");
    scanf("%d",&min);
    while(min > pow(2,var)){
        printf("Input tidak valid\n");
        printf("\nMasukkan jumlah minterms: ");
        scanf("%d",&min);
    }

    i=min-1;

    temp=(struct Node*)malloc(sizeof(struct Node));
    primeNode=temp;

    printf("\nMasukkan minterms\n\n");
    scanf("%d",&temp->data[0]);                     //first minterm is stored
    while(temp->data[0] > pow(2,var)-1){
        printf("Input tidak valid\n");
        printf("\nMasukkan minterms\n\n");
        scanf("%d",&temp->data[0]);
    }
    j=temp->data[0];
    temp->sumOne=0;
    x=var;
    k=0;
    while(x--)      //converts minterm to binary notation
    {
        if(j%2==1)
        {
            temp->bin[k]=1;
            temp->sumOne++;
        }
        else
        {
            temp->bin[k]=0;
        }
        j=j/2;
        k++;
    }
    while(i--)      //rest of the minterms are stored
    {
        temp=temp->next=(struct Node*)malloc(sizeof(struct Node));
        scanf("%d",&temp->data[0]);
        while(temp->data[0] > pow(2,var)-1){
            printf("Input tidak valid\n");
            printf("\nMasukkan minterms\n\n");
            scanf("%d",&temp->data[0]);
        }
        j=temp->data[0];
        temp->sumOne=0;
        x=var;
        k=0;
        while(x--)
        {
            if(j%2==1)          //converts the minterms to binary notation
            {
                temp->bin[k]=1;
                temp->sumOne++;       //the number of ones in binary notation
            }
            else
            {
                temp->bin[k]=0;
            }
            j=j/2;
            k++;
        }

    }
    temp->next=NULL;
}

void initial_implicants(struct Node* ptr)       //inisialisasi tiap minterm sebagai prime implicants
{
    struct Node* temp;
    temp=ptr;
    while(temp!=NULL)
    {
        temp->isimplicant=1;
        temp=temp->next;
    }
}


//Fungsi Search Index dari Nilai Minimum
int searchIdxMin(struct Node* head){
    int nilaiMin = 10000000;
    int idxMin = 0;
    int idx = 0;
    while(head != NULL){
        if (head->data[0] < nilaiMin){
            nilaiMin = head->data[0];
            idxMin = idx;
    }
    head = head->next;
    idx+=1;
}
    return idxMin;
}

//Fungsi Delete Node Linked List Berdasarkan Posisi Tertentu
void delNode(struct Node **head, int pos)
{

 if (*head == NULL){
      return;
 }

   struct Node* temp = *head;
    if (pos == 0){
        *head = temp->next;
        free(temp);
        return;
    }

    for (int i=0; temp!=NULL && i<pos-1; i++){
         temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL){
         return;
    }

    struct Node *skip = temp->next->next;
    free(temp->next);
    temp->next = skip;
}

//Fungsi Menambah Node pada Linked List Baru
void sort(struct Node **newHead, struct Node **oldHead){
    struct Node *nodeBaru = (struct Node*)malloc(sizeof(struct Node));
    struct Node *akhir = *newHead;
    struct Node *temp = *oldHead;

    for(int i=0; i<searchIdxMin(*oldHead); i++){
            temp = temp->next;
        }

    nodeBaru->data[0] = temp->data[0];

    nodeBaru->sumOne = temp->sumOne;

    for(int m=0; m<var; m++){
        nodeBaru->bin[m] = temp->bin[m];
    }
    delNode(oldHead, searchIdxMin(*oldHead));

    nodeBaru->next = NULL;

    if (*newHead == NULL){
        *newHead = nodeBaru;
        return;
    }

    while(akhir->next != NULL){
        akhir = akhir->next;
    }

    akhir->next = nodeBaru;
    return;
}

//Fungsi Print Linked List
void print(struct Node* head){
    struct Node* temp = head;
    while(temp != NULL){
        printf("%d ", temp->data[0]);
        temp = temp->next;
    }
}

void arrMinterms()       //array to store all the minterms
{
    int i=0;
    struct Node* temp;
    temp=primeNode;
    while(temp!=NULL)
    {
        minterms[i]=temp->data[0];
        i++;
        temp=temp->next;
    }
}

void groupby()       //where the minterms are sortd according to the number of ones
{
    int i,j,k=0;
    struct Node *temp,*skip;
    temp=save=primeNode;
    primeNode=skip=(struct Node*)malloc(sizeof(struct Node));
    for(i=0;i<=var;i++)
    {
        temp=save;
        while(temp!=NULL)
        {
            if(temp->sumOne==i)       //minterms are sortd according to no. of ones , first 0 ones then 1 ones... and so on
            {
                skip->data[0]=temp->data[0];
                k++;
                for(j=0;j<var;j++)
                {
                    skip->bin[j]=temp->bin[j];
                }
                skip->sumOne=temp->sumOne;
                skip=skip->next=(struct Node*)malloc(sizeof(struct Node));
            }
            temp=temp->next;
        }
    }
    minterms[k]=-1;
    skip->next=NULL;
}

void simplificationTab()     //for displaying the various column with pairings
{
    int i;
    struct Node* temp;
    temp=primeNode;
    printf("\n\n  Tabel Penyederhanaan %d\n",number); //number=taabel ke-
    printf("--------------------------\n\n");
    while(temp->next!=NULL)
    {
        printf("%d\t",temp->data[0]);
        for(i=var-1;i>=0;i--)
        {
            printf("%d",temp->bin[i]);
        }
        temp=temp->next;
        printf("\n");
    }
    temp->next=NULL;
    number++;
}

void removeLast(struct Node* ptr)         //reducing the number of Nodes in a list with one extra Node
{
    struct Node* temp;
    temp=ptr;
    while(temp->next->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=NULL;
}

void groupMore()    //grouping based on difference in binary notation
{
    int i,count,k,j,x;
    struct Node *temp,*skip,*p,*imp;
    check=0;
    if(columns==2)      //for second column
    {
        imp=impNode=(struct Node*)malloc(sizeof(struct Node));
        p=head;
    }
    else        //for other columns
    {
        imp=save;
        primeNode=head;
        p=head=(struct Node*)malloc(sizeof(struct Node));
    }
    temp=primeNode;
    initial_implicants(primeNode);
    printf("\n\n  Tabel Penyederhanaan %d\n",number); //number tells us which column is being printed
    printf("--------------------------\n\n");
    while(temp!=NULL)
    {
        skip=temp->next;
        while(skip!=NULL)
        {
            count=0;
            if(skip->sumOne-temp->sumOne==1)        //if two terms differ in their no. of ones by one
            {
                for(i=0;i<var;i++)
                {
                    if(temp->bin[i]!=skip->bin[i])
                    {
                        k=i;            //the place in which they differ is noted
                        count++;
                    }
                }
            }
            if(count==1)        //checks if the two terms differ by one place in binary notation
            {
                temp->isimplicant=0;        //if they do then they are not a prime implicant
                skip->isimplicant=0;
                check++;
                for(i=0;i<var;i++)
                {
                    p->bin[i]=temp->bin[i];         //binary notation is stored
                }
                p->bin[k]=-1;
                x=0;
                for(j=0;j<columns/2;j++)            //data from first term is stored
                {
                    p->data[x]=temp->data[j];
                    x++;
                }
                for(j=0;j<columns/2;j++)            //data from second term is stored
                {
                    p->data[x]=skip->data[j];
                    x++;
                }
                p->sumOne=temp->sumOne;
                for(j=0;j<columns;j++)      //the pair formed is displayed
                {
                    printf("%d,",p->data[j]);
                }
                printf("\b ");
                printf("\t");
                for(i=var-1;i>=0;i--)
                {
                    if(p->bin[i]==-1)
                        printf("-");
                    else
                        printf("%d",p->bin[i]);
                }
                printf("\n");
                p=p->next=(struct Node*)malloc(sizeof(struct Node));           // one extra Node that is to be deleted
            }
            skip=skip->next;
        }
        temp=temp->next;
    }
    p->next=NULL;
    if(check!=0)
    {
        removeLast(head);     //extra Node is deleted
    }
    temp=primeNode;
    while(temp!=NULL)           //for selecting the prime implicants
    {
        if(temp->isimplicant==1)        // if term is a prime implicant it is stored separately in list with head pointer impNode
        {
            i=0;
            for(i=0;i<columns/2;i++)
            {
                imp->data[i]=temp->data[i];
            }
            imp->data[i]=-1;
            for(i=0;i<var;i++)
            {
                imp->bin[i]=temp->bin[i];
            }
            imp=imp->next=(struct Node*)malloc(sizeof(struct Node));
        }
        temp=temp->next;
    }
    save=imp;
    columns=columns*2;
    number++;
}

void showImpli()       //displays the implicants
{
    int i=0;
    struct Node* temp;
    temp=impNode;
    printf("\n\nPrime Implicants : \n\n");
    while(temp!=NULL)
    {
        i=0;
        i=var-1;
        while(i>=0)     //displays the binary notation
        {
            if(temp->bin[i]==-1)
            {
                printf("-");
            }
            else
            {
                printf("%d",temp->bin[i]);
            }
            i--;
        }
        printf("\t\t");
        i=0;
        while(temp->data[i]!=-1)        //displays the minterm pairs
        {
            printf("%d,",temp->data[i]);
            i++;
        }
        printf("\b ");
        temp=temp->next;
        printf("\n\n");
        counter++;
    }
}


void reduce()          //reduces the terms that occur more than once to a single
{
    int common=0,i;
    struct Node *temp1,*temp2,*temp3;
    temp1=temp2=impNode;
    while(temp1!=NULL)
    {
        temp2=temp1->next;
        while(temp2!=NULL)
        {
            common=0;
            for(i=0;i<var;i++)          //if their binary notation is same one will be deleted
            {
                if(temp2->bin[i]==temp1->bin[i])
                {
                    common++;
                }
            }
            if(common==var)
            {
                temp3=impNode;
                while(temp3->next!=temp2)      //the repeated term is deleted
                {
                    temp3=temp3->next;
                }
                temp3->next=temp2->next;
                temp2=temp3;
            }
            temp2=temp2->next;
        }
        temp1=temp1->next;
    }
}

void initial_var()            //stores variables(alphabets)
{
    int i;
    for(i=0;i<26;i++)
    {
        a[i]=65+i;      //variables
    }
}

void changetoVar()          //it converts the binary notation of each term to variables
{
    int i,j;
    struct Node* temp;
    temp=last;
    while(temp!=NULL)
    {
        j=0;
        //printf("Code ASCII\n");
        for(i=0;i<var;i++)
        {
            if(temp->bin[i]==0)
            {
                temp->abc[j]=a[i];
                j++;
                temp->abc[j]=39;
                j++;
            }
            if(temp->bin[i]==1)
            {
                temp->abc[j]=a[i];
                j++;
            }
        }
        temp=temp->next;
    }
}

void finalResult(struct Node* head)         //displays the minimized function in SOP form
{
    struct Node* temp;
    temp=head;
    printf("\n\nF = ");
    while(temp!=NULL)
    {
        printf("%s",temp->abc);
        if(temp->next!=NULL)
        {
            printf(" + ");
        }
        temp=temp->next;
    }
    printf("\n\n");
}

void showTable()         //function for creating prime implicants table as well as selecting essential prime implicants
{
    struct Node* temp;
    int i,j,k,y,a=0;
    for(i=0;i<counter;i++)
    {
        for(j=0;j<=maxMinterms;j++)
        {
            tab[i][j]=0;           //0 or - is placed in all places of a table
        }
    }
    i=0;
    j=0;
    k=0;
    temp=impNode;
    while(temp!=NULL)
    {
        k=0;
        while(temp->data[k]!=-1)
        {
            tab[i][temp->data[k]]=1;  // 1 or X is placed for the column with same index as that of the number in the pair
            k++;
        }
        i++;
        temp=temp->next;
    }
    printf("\n\n");
    for(int z=0;z<round(min/2);z++){
        printf("\t");
    }
    printf("Prime Implicants Table\n\n\n");
    temp=impNode;
    i=0;
    printf(" ");
    while(minterms[i]!=-1)
    {
        printf("%d\t",minterms[i]);         //the minterms are displayed in row
        i++;
    }
    printf("|\tMinterms\n");
    for(int z=0;z<(2*min);z++){
        printf("====");
    }
    for(int z=0;z<=24;z++){
        printf("=");
    }

    printf("\n\n");


    for(i=0;i<counter;i++)          //X and - are placed for the terms with corresponding minterm values
    {
        printf(" ");
        a=0;
        for(j=0;j<=maxMinterms;j++)
        {

            if(j==minterms[a])
            {
                //printf("checkpoint5!!\n\n");
                if(tab[i][j]==0)
                {
                    printf("-");
                }
                if(tab[i][j]==1)
                {
                    printf("X");
                }
                printf("\t");
                a++;
            }
        }

        printf("|  ");
        y=0;
        while(temp->data[y]!=-1)        //prints the minterm pair
        {
            printf("%d,",temp->data[y]);
            y++;
        }
        printf("\b ");
        temp=temp->next;
        printf("\n");
        for(int z=0;z<(2*min);z++){
        printf("----");
        }
        for(int z=0;z<24;z++){
        printf("-");
        }
        printf("\n\n");

    }
    printf("\n\n");
}

void implicant()     //after finding the essential prime implicants other terms necessary are marked
{
    no=0;           //to check if any term is found in each iteration
    int count1=0,count2=0;
    int i,j;
    for(i=0;i<counter;i++)
    {
        count1=0;
        for(j=0;j<=maxMinterms;j++)
        {
            if(tab[i][j]==1)       //no. of X's or 1's are calculated
            {
                no++;
                count1++;
            }
        }
        if(count1>count2)       //to find the term with maxMintermsimum X's in a row
        {
            essential[t]=i;
            count2=count1;
        }
    }
    for(j=0;j<=maxMinterms;j++)           //removing the X's in the row as well a those X's which are in same column
    {
        if(tab[essential[t]][j]==1)
        {
            for(i=0;i<counter;i++)
            {
                tab[i][j]=0;
            }
        }
    }
    t++;
    essential[t]=-1;
    if(no>0)            //if one or more terms is found the function is called again otherwise not
    {
        implicant();
    }
}

void finalNode()          //in this function all the terms in the minimized expression are stored in a linked list
{
    int i=0,j,c=0,x;
    struct Node *temp,*ptr;
    last=temp=(struct Node*)malloc(sizeof(struct Node));
    while(essential[i]!=-1)
    {
        ptr=impNode;
        x=essential[i];
        for(j=0;j<x;j++)        //so that pointer points to the Node whose index was stored in array named essential
        {
            ptr=ptr->next;
        }
        j=0;
        while(ptr->data[j]!=-1)         // the data of the Node is stored
        {
            temp->data[j]=ptr->data[j];
            j++;
        }
        temp->data[j]=-1;
        int k=0;
        for(j=var-1;j>=0;j--)          //the binary code is stored
        {
            temp->bin[j]=ptr->bin[k];
            k++;
        }
        temp=temp->next=(struct Node*)malloc(sizeof(struct Node));
        i++;
        //c++;
    }
    temp->next=NULL;
}

int main()
{
    initial_var();
    printf("=======================================================================================\n\n");
    printf("\t\t\t\t Program Minimasi Logika\n\t\t\tdengan Menerapkan Algoritma Quine McCluskey\n\n");
    printf("=======================================================================================\n");
    printf("Ketentuan Input:\n");
    printf("1. Jumlah variabel maksimal sebanyak 26\n2. Jumlah minterms maksimal sebanyak 2^variabel\n3. Elemen minterms tidak boleh bernilai lebih dari (2^variabel)-1\n");
    inputData();
    new = NULL;
    for(int o=0; o<min; o++){
        sort(&new, &primeNode);
    }
    primeNode = new;

    arrMinterms();
    maxMinterms = minterms[min-1];
    groupby();
    simplificationTab();
    removeLast(primeNode);
    head=(struct Node*)malloc(sizeof(struct Node));
    while(check>0)
    {
        groupMore();
    }
    save->next=NULL;           //storing null value in link field of list storing prime implicants
    printf("Kalkulasi selesai\n\n");
    removeLast(impNode);
    reduce();
    showImpli();
    showTable();
    implicant();
    finalNode();
    removeLast(last);
    changetoVar();
    finalResult(last);
    return 0;
}
