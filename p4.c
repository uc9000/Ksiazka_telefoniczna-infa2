#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct dane{
	char imie[50];
	char nazwisko[50];
	char nr[15];
	char grupa[50];
};

struct lista{	
	struct dane dl;
	struct lista* next_ptr;
	struct lista* prev_ptr;
};

int dajInt() {
	unsigned int i = 0;
	int liczba, loop = 0;
	char buff[1000];
	do {
		scanf("%999s", buff);
		for (i = 0; i < strlen(buff); i++) {
			if (!(isdigit(buff[i]) || buff[i] == '-') || buff[i] == '.') {
				printf("Podaj liczbe calkowita! : ");
				loop = 1;
				break;
			}
			loop = 0;
		}
	} while (loop == 1 || buff[0] == '\0' || buff[0] == '\n');
	liczba = atol(buff);
	return liczba;
}

int tak_czy_nie() {
	short unsigned int q = 0;
	do {
		printf("	1) Tak\n	2) Nie\n");
		q = dajInt();
		if (q == 2) {
			return 0;
		}
		else if (q == 1) {
			return 1;
		}
		else {
			printf("Podaj liczbe 1 lub 2\n");
			continue;
		}

	} while (!(q == 1 || q == 2));
	printf("\n");
	return 0;
}

void dajNazwe(char* nazwa_pliku, char* rozsz) {
	int i;
	short unsigned int loop = 0;
	char t = '\0';
	char buff[1000];
	nazwa_pliku[0] = '\0';
	do {
		scanf("%999s", buff);
		if (strlen(buff) > 50) {
			printf("Nazwa pliku jest za dluga!\n");
			continue;
		}
		for (i = 0; i < strlen(buff); i++) {
			t = buff[i];
			if (t == '/' || t == '\\' || t == '$' || t == '.') {
				printf("Nazwa zawiera niedozwolone znaki!\n");
				loop = 1;
				break;
			}
			loop = 0;
		}
	} while (loop == 1 || buff[0] == '\0' || buff[0] == '\n');
	sprintf(nazwa_pliku, "%s.%s", buff, rozsz);
	return;
}

void get_string(char* x, int max) {
	char buff[1000];
	x[0] = '\0';
	do {
		fgets(buff, 999, stdin);
		buff[strlen(buff) - 1] = '\0';
		if (strlen(buff) > max) {
			printf("Podany ciag znakow jest za dlugi!\n");
			continue;
		}
	} while (buff[0] == '\0' || buff[0] == '\n');
	sprintf(x, "%s", buff);
	return;
}

void lowercase_string(char* x){
	int i = 0;
	for(i = 0; i < strlen(x); i++){
		x[i] = tolower(x[i]);
	}
}

void kontakty_zapis(struct lista* a) {
	char nazwa_pliku[50];
	struct lista* iter = a;
	FILE* wsk_pliku;
	printf("Podaj nazwe pliku csv do zapisu:\n");
	dajNazwe(nazwa_pliku, "csv");
	printf("Czy chcesz zapisac plik '%s'\n", nazwa_pliku);
	if (!tak_czy_nie())
		return;
	wsk_pliku = fopen(nazwa_pliku, "w");
	while (iter != NULL) {
		fprintf(wsk_pliku, "%s;%s;%s;%s\n", iter->dl.imie, iter->dl.nazwisko, iter->dl.nr, iter->dl.grupa);
		iter = iter->next_ptr;
	}
	fclose(wsk_pliku);
	printf("Zapisano plik: '%s'\n", nazwa_pliku);
}

void set_data(struct dane* d, char* imie, char* nazwisko, char* nr, char* grupa){
	strcpy(d->imie, imie);
	strcpy(d->nr, nr);
	strcpy(d->nazwisko, nazwisko);
	strcpy(d->grupa, grupa);
}

void print_d(struct dane d){
	printf("Imie: '%s'; Nazwisko: '%s'; Grupa: '%s'; Nr: '%s'\n", d.imie, d.nazwisko, d.grupa, d.nr);
}

void clean_list(struct lista** a){
	struct lista* iter = *a;
	struct lista* tmp;
	if(*a == NULL){
		return;
	}
	while(iter != NULL){
		tmp = iter->next_ptr;
		free(iter);
		iter = tmp;
	}
	*a = NULL;
}

void push_back(struct dane d, struct lista** lista_ptr){
	struct lista* new_ptr = malloc(sizeof(struct lista));
	struct lista* iter = *lista_ptr;
	strcpy(new_ptr->dl.imie, d.imie);
	strcpy(new_ptr->dl.nr, d.nr);
	strcpy(new_ptr->dl.nazwisko, d.nazwisko);
	strcpy(new_ptr->dl.grupa, d.grupa);
    new_ptr->next_ptr = NULL;
	if (*lista_ptr == NULL){
		new_ptr->prev_ptr = NULL;
		*lista_ptr = new_ptr;
		return;
	}	
	while(iter->next_ptr != NULL){
		iter = iter->next_ptr;
	}
    iter->next_ptr = new_ptr;
	new_ptr->prev_ptr = iter;
    //print_d(new_ptr->dl);
    return;
}

int listlen(struct lista* a){
	int i = 0;
	struct lista* iter = a;
	while(iter != NULL){    
		iter = iter->next_ptr;
		i++;	
	}
	return i;
}

void remove_at(int index, struct lista** a){
	int i = 0;
	struct lista* iter = *a;
	if (*a == NULL || index >= listlen(*a)){
		return;
	}
	for(i = 0; i < index; i++){
		iter = iter->next_ptr;		
	}
	if(iter->prev_ptr != NULL){
		iter->prev_ptr->next_ptr = iter->next_ptr;
	}
	else{
		*a = iter->next_ptr;
		return;
	}
	if(iter->next_ptr != NULL){
		iter->next_ptr->prev_ptr = iter->prev_ptr;
	}
	free(iter);
	return;
}

void print_list(struct lista* a){
	int i = 1;
	struct lista* iter = a;
    if(a == NULL){
        printf("Lista jest pusta.\n");
        return;
    }
	while(iter != NULL){
        printf(" [%d] Imie: %s, Nazwisko: %s, Grupa: %s, Nr: %s\n", i, iter->dl.imie, iter->dl.nazwisko, iter->dl.grupa, iter->dl.nr);        
		iter = iter->next_ptr;	
		i++;
	}
	puts("");
}

void kontakty_odczyt(struct lista** a) {
	char nazwa_pliku[50];
	char buff[200];
	struct dane d;
	FILE* wskPliku;
	while(1){
		printf("Podaj nazwe pliku csv do odczytu:\n");
		dajNazwe(nazwa_pliku, "csv");
		wskPliku = fopen(nazwa_pliku, "r");
		if(wskPliku != NULL)
			break;
		printf("Blad! Plik o nazwie '%s' nie istnieje", nazwa_pliku);
	}
	while (fgets(buff, 199, wskPliku) != NULL) {
		sscanf(buff, "%[^;];%[^;];%[^;];%[^\n]", d.imie, d.nazwisko, d.nr, d.grupa);
		push_back(d, a);
	}
	fclose(wskPliku);
	puts("Odczytano plik %s", nazwa_pliku);
	return;
}

void sort_by(struct lista** s, char* w){
	int i = 0;
	char* a;
	char* b;
	struct lista* iter = *s;
	struct dane tmp;
	if(*s == NULL || w[0] == '\0'){
		return;
	}
	for(i = 0; i < listlen(*s); i++){		
		iter = *s;
		while(iter != NULL && iter->next_ptr != NULL){		
			if(!strcmp(w, "imie")){
				a = iter->dl.imie;
				b = iter->next_ptr->dl.imie;
			}
						
			if(!strcmp(w, "nazwisko")){
				a = iter->dl.nazwisko;
				b = iter->next_ptr->dl.nazwisko;
			}						
			if(!strcmp(w, "nr")){
				a = iter->dl.nr;
				b = iter->next_ptr->dl.nr;
			}			
			if(!strcmp(w, "grupa")){
				a = iter->dl.grupa;
				b = iter->next_ptr->dl.grupa;
			}
			if(strcmp(a, b) > 0){	
				tmp = iter->dl;
				iter->dl = iter->next_ptr->dl;
				iter->next_ptr->dl = tmp;
			}
			iter = iter->next_ptr;
		}
	}
}

void get_at(int index, struct lista* a, struct dane* d){
	int i = 0;
	struct lista* iter = a;
	while(iter != NULL && i < index){        
		iter = iter->next_ptr;
		i++;
	}
	memcpy(d, &iter->dl, sizeof(struct dane));
}

int add_contact(struct dane* d){
	while(1){
		printf("Podaj imie: ");
		get_string(d->imie, 50);
		printf("Podaj nazwisko: ");
		get_string(d->nazwisko, 50);
		printf("Podaj nr: ");
		get_string(d->nr, 15);
		printf("Podaj grupe: ");
		get_string(d->grupa, 50);
		printf("Czy zapisac utworzony kontakt na liscie?\n");
		if(tak_czy_nie()){
			return 1;
		}
		else{
			return 0;
		}
	}	
}

void search(struct lista* kontakty){
	char bufor[50];
	char tmp[100];
	int wybor = 0;
	struct lista* iter;
	struct lista* lista_sorted = NULL;
	char* char_ptr = NULL;
	if(kontakty == NULL){
		puts("Lista jest pusta!");
		return;
	}
	do{
		puts("1) Szukaj kontaktu po imieniu i nazwisku");
		puts("2) Szukaj kontaktu po grupie");
		while(1){
			wybor = dajInt();
			if(wybor <= 2 && wybor >= 1)
				break;
			printf("Podaj liczbe 1 lub 2!\n");
		}
		printf("Podaj szukany ciag znakow: ");
		get_string(bufor, 99);
		//lowercase_string(bufor);
		printf("bufor = %s\n", bufor);
		iter = kontakty;
		while(iter != NULL){			
			if (wybor == 1){
				sprintf(tmp, "%s %s", iter->dl.imie, iter->dl.nazwisko);
			}
			else{
				strcpy(tmp, iter->dl.grupa);
			}			
			lowercase_string(tmp);
			//printf("tmp = %s\n", tmp);
			char_ptr = strstr(tmp, bufor);
			if(char_ptr != NULL){
				push_back(iter->dl, &lista_sorted);
			}		
			iter = iter->next_ptr;			
		}
		print_list(lista_sorted);
		puts("Czy chcesz wyszukac na nowo?");
		clean_list(&lista_sorted);
	}while(tak_czy_nie());
	return;	
}

void menu(){
	struct lista* kontakty = NULL;
	struct dane d;
	int wybor1 = 0, wybor2 = 0;
	char sorted_by[20];
	sorted_by[0] = '\0';
	while(wybor1 != 9){
		switch(wybor1){
			case 0:
				puts("1) Odczytaj kontaky z pliku .csv");
				puts("2) Zapisz kontakty do pliku .csv");
				puts("3) Wyswietl liste kontaktow");
				puts("4) Sortuj liste kontaktow");
				puts("5) Wyszukaj kontakt");
				puts("6) Wyczysc liste");
				puts("7) Usun kontakt");
				puts("8) Dodaj kontakt");
				puts("9) Zakoncz");
				while(1){
					wybor1 = dajInt();
					if(wybor1 <= 9 && wybor1 >= 1)
						break;
					printf("Podaj liczbe od 1 do 9!\n");
				}
				break;
			case 1:
				kontakty_odczyt(&kontakty);
				wybor1 = 0;
				break;
			case 2:
				kontakty_zapis(kontakty);
				wybor1 = 0;
				break;
			case 3:
				print_list(kontakty);
				wybor1 = 0;
				break;
			case 4:
				while(1){
					if (wybor2 == 0){
						puts("1) Sortuj po imieniu");
						puts("2) Sortuj po nazwisku");
						puts("3) Sortuj po grupie");
						puts("4) Sortuj po numerze");
						puts("5) Anuluj");
						while(1){
							wybor2 = dajInt();
							if(wybor2 <= 5 && wybor2 >= 1)
								break;
							printf("Podaj liczbe od 1 do 5!\n");
						}
					}
					if (wybor2 == 1){
						strcpy(sorted_by, "imie");
						sort_by(&kontakty, sorted_by);
						wybor2 = 0;
						break;
					}
					if (wybor2 == 2){
						strcpy(sorted_by, "nazwisko");
						sort_by(&kontakty, sorted_by);
						wybor2 = 0;
						break;
					}
					if (wybor2 == 3){
						strcpy(sorted_by, "grupa");
						sort_by(&kontakty, sorted_by);
						wybor2 = 0;
						break;
					}
					if (wybor2 == 4){
						strcpy(sorted_by, "nr");
						sort_by(&kontakty, sorted_by);
						wybor2 = 0;
						break;
					}
				}
				wybor1 = 0;
				break;
			case 5:
				search(kontakty);
				wybor1 = 0;
				break;
			case 6:
				printf("Czy napewno chcesz usunac cala liste kontaktow?\n");
				if(tak_czy_nie()){
					clean_list(&kontakty);
				}				
				wybor1 = 0;
				break;
			case 7: //usun kontakt
				print_list(kontakty);
				printf("Wybierz nr obrazu do usuniecia: ");
				while(1){
					wybor2 = dajInt();
					if(wybor2 <= listlen(kontakty) && wybor2 > 0){
						break;
					}
					else{
						printf("Wybierz obraz dostepny z listy!\n");
						continue;
					}
				}				
				remove_at(wybor2 - 1, &kontakty);
				wybor2 = 0;
				wybor1 = 0;
				break;
			case 8: //dodaj kontakt
				if (add_contact(&d)){
					push_back(d, &kontakty);
					sort_by(&kontakty, sorted_by);
					printf("Dodano %s %s do kontaktow\n", d.imie, d.nazwisko);
				}
				wybor1 = 0;
				break;
			
			case 9:
				printf("Czy napewno chcesz zakonczyc dzialanie programu?\n");
				if(!tak_czy_nie()){
					wybor1 = 0;
					break;
				}
				clean_list(&kontakty);
				return;			
		}
	}
}

int main(){
	menu();
	printf("\nProgram zakonczyl dzialanie.\n");
	return 0;
}