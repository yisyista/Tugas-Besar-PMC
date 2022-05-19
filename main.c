#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Node
{
    int data[257], bin[26], sumOne, isimplicant;
    char var[26];
    struct Node* next;
};

struct Node *root,*head,*improot,*save,*fin,*new;
int var,min,number=1,columns=2,check=1,limit,tab[100][100],counter=0,essential[1000],t=0,no=0,minterms[1000];
char a[26],b[26];       //

void groupby();          //
void simplificationTab();            //
void groupMore();           //
void delNode(struct Node*);            //
void showImpli();              //the implicants are displayed
void initial_implicants(struct Node*);          //
void reduce();                 //
void initial_var();       //
void changetoVar();             //
void showTable();        //
void finalResult();                //
void implicant();        //
void finalNode();     //
void saveMinterms();      //
int searchMin();
int searchIdxMin(struct Node*);
void deleteNode(struct Node**,int);
void sort(struct Node**,struct Node**);


int main()
{
    int i,j,k,x;
    struct Node* temp;
    printf("\nMasukkan jumlah variabel : ");       //no. of variables and minterms are recorded
    scanf("%d",&var);
    printf("\nMasukkan jumlah minterms: ");
    scanf("%d",&min);
    i=min-1;

    temp=(struct Node*)malloc(sizeof(struct Node));
    root=temp;

    printf("\nMasukkan minterms\n\n");
    scanf("%d",&temp->data[0]);                     //first minterm is stored
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

    new = NULL;
    for(int o=0; o<min; o++){
        sort(&new, &root);
    }
    root = new;

    saveMinterms();
    limit = minterms[min-1];
    groupby();
    simplificationTab();
    delNode(root);
    head=(struct Node*)malloc(sizeof(struct Node));
    while(check>0)
    {
        groupMore();
    }
    save->next=NULL;           //storing null value in link field of list storing prime implicants
    printf("Kalkulasi selesai\n\n");
    delNode(improot);
    reduce();
    showImpli();
    initial_var();
    showTable();
    implicant();
    finalNode();
    delNode(fin);
    changetoVar();
    finalResult();
    return 0;
}

//Fungsi Search Nilai Minimum dari Linked List
int searchMin(struct Node* head){
    int nilaiMin = 10000000;
    while(head != NULL){
        if (head->data[0] < nilaiMin){
            nilaiMin = head->data[0];
    }
    head = head->next;

}
    return nilaiMin;
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
void deleteNode(struct Node **head, int pos)
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
    deleteNode(oldHead, searchIdxMin(*oldHead));

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


void saveMinterms()       //array to store all the minterms
{
    int i=0;
    struct Node* temp;
    temp=root;
    while(temp!=NULL)
    {
        minterms[i]=temp->data[0];
        i++;
        temp=temp->next;
    }
}

void groupby()       //where the minterms are sortd according to the number of ones
{
    int i,count=0,j,k=0;
    struct Node *temp,*skip;
    temp=save=root;
    root=skip=(struct Node*)malloc(sizeof(struct Node));
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
    int i,j=min;
    struct Node* temp;
    temp=root;
    printf("\n\n  Tabel Penyederhanaan %d\n",number); //number tells us which column is being printed
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

void delNode(struct Node* ptr)         //reducing the number of Nodes in a list with one extra Node
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
        imp=improot=(struct Node*)malloc(sizeof(struct Node));
        p=head;
    }
    else        //for other columns
    {
        imp=save;
        root=head;
        p=head=(struct Node*)malloc(sizeof(struct Node));
    }
    temp=root;
    initial_implicants(root);
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
        delNode(head);     //extra Node is deleted
    }
    temp=root;
    while(temp!=NULL)           //for selecting the prime implicants
    {
        if(temp->isimplicant==1)        // if term is a prime implicant it is stored separately in list with head pointer improot
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
    temp=improot;
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

void initial_implicants(struct Node* ptr)       //initializing each term as a prime implicant
{
    struct Node* temp;
    temp=ptr;
    while(temp!=NULL)
    {
        temp->isimplicant=1;
        temp=temp->next;
    }
}

void reduce()          //reduces the terms that occur more than once to a single
{
    int common=0,i;
    struct Node *temp1,*temp2,*temp3;
    temp1=temp2=improot;
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
                temp3=improot;
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
    temp=fin;
    while(temp!=NULL)
    {
        j=0;
        //printf("Code ASCII\n");
        for(i=0;i<var;i++)
        {
            if(temp->bin[i]==0)
            {
                temp->var[j]=a[i];
                j++;
                temp->var[j]=39;
                j++;
            }
            if(temp->bin[i]==1)
            {
                temp->var[j]=a[i];
                j++;
            }
        }
        temp=temp->next;
    }
}

void finalResult()         //displays the minimized function in SOP form
{
    struct Node* temp;
    temp=fin;
    printf("\n\nF = ");
    while(temp!=NULL)
    {
        printf("%s",temp->var);
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
    int i,j,k,m,n,x,y,count=0,count2=0,a=0;
    for(i=0;i<counter;i++)
    {
        for(j=0;j<=limit;j++)
        {
            tab[i][j]=0;           //0 or - is placed in all places of a table
        }
    }
    i=0;
    j=0;
    k=0;
    temp=improot;
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
    temp=improot;
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
        for(j=0;j<=limit;j++)
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
        for(j=0;j<=limit;j++)
        {
            if(tab[i][j]==1)       //no. of X's or 1's are calculated
            {
                no++;
                count1++;
            }
        }
        if(count1>count2)       //to find the term with maximum X's in a row
        {
            essential[t]=i;
            count2=count1;
        }
    }
    for(j=0;j<=limit;j++)           //removing the X's in the row as well a those X's which are in same column
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
    fin=temp=(struct Node*)malloc(sizeof(struct Node));
    while(essential[i]!=-1)
    {
        ptr=improot;
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
        c++;
    }
    temp->next=NULL;
}

