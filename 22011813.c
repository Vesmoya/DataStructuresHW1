/*Volkan Tayranoglu 22011813 Veri Yapilari ve Algoritmalar �devi*/
#include <stdio.h>
#include <stdlib.h>

//Struct yapisinin tanimi
typedef struct Node{
    char sayfa[50];
    int istek;
    struct Node* prev;
    struct Node* next;
}Node;


//yeni node'u listeye ekleyen fonksiyon
Node* addNode(char* sayfa,Node** head){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode==NULL){
        printf("bellek ayrilamadi!");
        exit(1);
    }
    if(newNode->sayfa == NULL){
        printf("bellek ayrilamadi!");
        exit(1);
    }
    strcpy(newNode->sayfa,sayfa);//Girilen sayfa ismine node'a yaziyoruz.
    newNode->istek = 1;//Ilk defa olacagi icin istek degerini 1 atiyoruz.
    newNode->prev=NULL;
    newNode->next=*head;//Yeni node'u basa yerlestiyiroruz.
    if(*head != NULL){
        (*head)->prev = newNode;

    }
    *head = newNode;
    return newNode;
}
//Girilen sayfa isminin listedeki kontrolunu yapan fonksiyon
Node* sayfaArama(char* sayfa,Node *head){
    Node* current = head;
    while(current!=NULL){
        if(strcmp(sayfa,current->sayfa)==0){
            return current;//Girilen sayfa degeri ile listede eslesme varsa o dugumu donduruyoruz.
        }
        current=current->next;
    }
    return NULL;//Sayfa ismi bulunamazsa NULL donduruyoruz.
}
//Esik degerini asan node'u head olarak atayan fonksiyon
void makeHead(Node* newHead,Node** head){
    if(newHead!=*head){ //Node zaten head degilse islem yapiyoruz.
        if(newHead->next !=NULL){ //son eleman degilse islem yapiyoruz.
            newHead->next->prev = newHead->prev;
            newHead->prev->next = newHead->next;
        }else { //son elemansa islem yapiyoruz.
            newHead->prev->next = NULL;//son elemansa onceki elemanin next ini NULL yapmamiz yeterli.
        }

        newHead->prev=NULL;//Listeden kopardigimiz dugumu basa koyuyoruz.
        newHead->next=*head;
        *head=newHead;
    }
}
//Liste uzunlugu asildiginda son node'u silen fonksiyon
void deleteLastNode(Node** head){
    Node* current = *head;
    while (current->next != NULL){ //Son dugume ulasiyoruz bu dongude.
        current=current->next;
    }
    current->prev->next=NULL; //Onceki dugumun sonraki degerini NULL yaparak dugumu siliyoruz.
    free(current);//Dugum bellegini serbest birakiyoruz.
}
//Dosyadan sayfa isimlerini alan fonksiyon
void readFromFileAndAddToList( int *ptrListLenght, int L, int T,Node** ptrHead)
{
    FILE *fp = fopen("pages.txt", "r");//Dosyadan bilgi almak icin dosyayi aciyoruz.
    char sayfaAdi[50];
    if(fp==NULL){
        printf("Dosya acilamadi.\n");
        exit(-1);
    }
    while(fscanf(fp,"%s",sayfaAdi) != EOF){
        *ptrListLenght = listOperations(sayfaAdi,*ptrListLenght,L,T,ptrHead); //Dosya sonuna ulasana kadar deger alip liste islemlerini yapiyoruz.
    }
    fclose(fp);
}
//Sayfa isimlerini kullanicidan alan fonksiyon
void readFromUserAndAddToList( int *ptrListLenght, int L, int T,Node** ptrHead)
{
    char sayfaAdi[50];
    printf("sayfa adlarini sirayla giriniz.\nSayfa girisini bitirmek icin exit yaziniz.\n");
    while(scanf("%s", sayfaAdi) == 1 && strcmp(sayfaAdi, "exit") != 0){ //Sayfa ismini alip ve ismin "exit" olup olmadigini kontrol ediyoruz.
        *ptrListLenght = listOperations( sayfaAdi, *ptrListLenght, L, T,ptrHead); //Gonderdigimiz isimle liste islemlerini yapip guncel liste uzunlugunu aliyoruz.
    }
}

//List islemlerini yapan fonksiyon
int listOperations(char* sayfaAdi,int listLenght,int L,int T,Node** head){
    Node* tmpNode;
    tmpNode = sayfaArama(sayfaAdi,*head);//Girilen sayfa ismi listede var mi diye kontrol ediyoruz.
    if (tmpNode == NULL)
    {
        addNode(sayfaAdi,head);//Listede yoksa listeye ekliyoruz
        listLenght++;//Guncel liste uzunlugunu arttiriyoruz.
        if (listLenght > L)
        {
            deleteLastNode(head);//Liste uzunlugu max uzunlugu gecerse son elemani siliyoruz.
        }
        printList(*head);//Listenin guncel halini ekrana yazdiriyoruz.
    }
    else
    {
        tmpNode->istek++;//Girilen sayfa listede varsa istek degerini arttiriyoruz.
        if (tmpNode->istek > T)
        {
            makeHead(tmpNode,head);//Istek degeri esik degerini gecmisse listenin basina tasiyoruz.
        }
        printList(*head);//Listenin guncel halini ekrana yazdiriyoruz
    }
    return listLenght;//Guncel liste uzunlugunu donduruyoruz.
}
//Listeyi ekrana yazdiran fonksiyon
void printList(Node *head){
    Node* current=head;
    while(current!=NULL){
        printf("%s,%d--",current->sayfa,current->istek);
        current=current->next;
    }
    printf("\n");

}
//Listeyi silip bellegi bosaltan fonksiyon
void freeList(Node* head){
    Node* current = head;
    Node* tmp=NULL;
    while (current!=NULL){
        tmp=current; //Su anki node'u gecici olarak tutup listeden kaldiriyoruz.Sonra node'un bellegini bosaltiyoruz.
        current=current->next;
        free(tmp);
    }
}

int main(){
    int T,L;//Esik ve max liste uzunlugu
    Node* head = NULL;//Head in tanimi
    int listLenght=0;//Guncel liste uzunlugu
    int readType;//Okuma tipi kontrolunu yapan degisken
    int temizlikControl;//Listenin silinme sorgusunda kullanilan degisken
    printf("Maksimum liste uzunlugunu giriniz:\n");
    scanf("%d",&L);
    printf("istek icin esik degerini giriniz:\n");
    scanf("%d",&T);

    printf("sayfayi dosyadan okumak icin 1'i, kendiniz girmek icin 2'yi tuslayiniz.\n");
    scanf("%d",&readType);
    switch(readType){//Kullanici dosyadan okumak isterse case 1, kendisi girmek isterse case 2 ye yonlendiriliyor.
        case 1:
               readFromFileAndAddToList(&listLenght,L,T,&head);
               printf("Listenin son hali:\n");
               printList(head);
               printf("Listeyi temizlemek icin 1 i tuslayiniz.\n");
               scanf("%d",&temizlikControl);
               if(temizlikControl==1){
                freeList(head);
                printf("Liste basariyla temizlendi.\n");
               }
               break;
        case 2:
               readFromUserAndAddToList(&listLenght,L,T,&head);
               printf("Listenin son hali:\n");
               printList(head);
               printf("Listeyi temizlemek icin 1 i tuslayiniz.\n");
               scanf("%d",&temizlikControl);
               if(temizlikControl==1){
                freeList(head);
                printf("Liste basariyla temizlendi.\n");
               }
               break;
        default:
               printf("Gecersiz no girdiniz.\n");
    }

    return 0;
}
