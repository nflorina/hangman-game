#include <stdio.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <curses.h>
#include <sys/select.h>

static void game(int argc, char **argv);
static void the_big_menu(int argc, char **argv);
static void history(int argc, char **argv);

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define FOREVER 1
#define STR_MAX_LEN 100
#define S_TO_WAIT 2
#define MICROS_TO_WAIT	0
#define KEYBOARD 0
#define SELECT_ERROR -1
#define SELECT_EVENT 1
#define SELECT_NO_EVENT	0


int input()
{ // analizez daca am input in timp util

	int nfds, sel;
	fd_set read_descriptors;
	struct timeval timeout;
	nfds = 1;
	FD_ZERO(&read_descriptors);
	FD_SET(KEYBOARD, &read_descriptors);
	timeout.tv_sec = 7;

	while (FOREVER) {
		sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
		switch (sel) {
		case SELECT_EVENT:
			return 1; //daca se introduce ceva de la tastatura
			
		case SELECT_NO_EVENT:
			return 2; //daca nu se introduce +
                      //a trecut timeout-ul de 7s
		}
		FD_SET(KEYBOARD, &read_descriptors);
		timeout.tv_sec = 7;
	}
	
}

void history(int argc, char **argv){
    //optiune noua in cadrul meniului
    //contine stategii de joc si istoria lui

	int row, col;
	char c;
	initscr();
	clear();
	noecho();
	cbreak();
	start_color();
	getmaxyx(stdscr, row, col);
	init_pair(1, COLOR_BLACK, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);
	init_pair(3, COLOR_RED, COLOR_GREEN);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_BLACK, COLOR_YELLOW);
	attron(COLOR_PAIR(3));
	mvaddstr(0, 20, "History of Hangman");
	attron(COLOR_PAIR(4));
	mvaddstr(1, 10, "Press Q to exit!");
	attron(COLOR_PAIR(1));
	mvaddstr(2, 9, "<<The origins of Hangman are obscure meaning not discovered, but it ");
	mvaddstr(3, 2, "seems to have arisen in Victorian times,>> says Tony Augarde, author ");
	mvaddstr(4, 2, "of The Oxford Guide to Word Games. The game is mentioned in Alice ");
	mvaddstr(5, 2, "Bertha Gomme's <<Traditional Games>> in 1894 under the name <<Birds, >>");
	mvaddstr(6, 2, "Beasts and Fishes>>. The rules are simple; a player writes down the first");
	mvaddstr(7, 2, "and last letters of a word and another player guesses the letters in between.");
	mvaddstr(8, 2, "In other sources,[where?] the game is called <<Gallows>> or <<Hanger>>.");

	attron(COLOR_PAIR(4));
	mvaddstr(10, 20, "Strategy");
	attron(COLOR_PAIR(1));
	mvaddstr(12, 9, "The fact that the twelve most commonly occurring letters in the English");
	mvaddstr(13, 2, "are e-t-a-o-i-n-s-h-r-d-l-u (from most to least), along with other");
	mvaddstr(14, 2, "letter-frequency lists, are used by the guessing player to increase the odds");
	mvaddstr(15, 2, "when it is their turn to guess. On the other hand, the same lists can be");
	mvaddstr(16, 2, "used by the puzzle setter to stump their opponent by choosing a word which");
	mvaddstr(17, 2, "deliberately avoids common letters (e.g. rhythm or zephyr) or one that ");
	mvaddstr(18, 2, "contains rare letters (e.g. jazz).");
	mvaddstr(19, 9, "Another common strategy is to guess vowels first, as English only has");
	mvaddstr(20, 2, "five vowels (a, e, i, o, and u, while y may sometimes, but rarely, be used as");
	mvaddstr(21, 2, "vowel and almost every word has at least one. According to a 2010 study");
	mvaddstr(22, 2, "conducted by Jon McLoone for Wolfram Research, the most difficult ");
	mvaddstr(23, 2, "words to guess include jazz, buzz, hajj, faff, fizz, fuzz and variations.");
	refresh();

	while(FOREVER){
		c = getchar();
		if(c == 'Q')
			the_big_menu(argc, argv);
	}
	endwin();

}

//elementele disponbile din meniu
char *choices[] = { 
                        "New Game", 
                        "Resume Game", 
                        "Quit",  
                        "About Hangman",
                        (char *)NULL, 
                  }; 

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color); 

void the_big_menu(int argc, char **argv){
    //printez meniul principal

    	int row, col;
        getmaxyx(stdscr, row, col);
        //aflu coordonatele spatiale
        ITEM **my_items;       
        int c;                                        
        MENU *my_menu;        
        WINDOW *my_menu_win; 
        int n_choices, i; //nr elemente
        /* Initialize curses */ 
        initscr(); 
        clear();       
        start_color();        
        cbreak(); 
        noecho(); 
        keypad(stdscr,TRUE);        
        //culorile perechi
        init_pair(1, COLOR_BLACK, COLOR_CYAN); 
        init_pair(2, COLOR_MAGENTA, COLOR_RED);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        //creez itemii asociati meniului
        n_choices = ARRAY_SIZE(choices); // nr elemente
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *)); 
        //aloca pt nr elemente 
        for(i = 0; i < n_choices; ++i) 
                my_items[i] = new_item(choices[i], choices[i]); 
        //creez meniul propriu-zis

                my_menu = new_menu((ITEM **)my_items); 
        //creez fereastra meniului 
        my_menu_win = newwin(10, 40, 4, 4); // a creat fereastra
        keypad(my_menu_win, TRUE); 
        //setez fereastra principala si secundara
        set_menu_win(my_menu, my_menu_win); 
        bkgd(COLOR_PAIR(2));
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1)); 
        //aleg un semn pentru cuvantul ales
        set_menu_mark(my_menu, " * "); 
        //printez bordura si titlul meniului 
        box(my_menu_win, 0, 0); 
        print_in_middle(my_menu_win, 1, 0, 40, "My Menu", COLOR_PAIR(1));
        mvwaddch(my_menu_win, 2, 0, ACS_LTEE); 
        mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38); 
        //afisez meniul
        post_menu(my_menu);        
        wrefresh(my_menu_win);  

        //afisez titlul jocului sub forma mare
        //imi aleg perechea de culori
        attron(COLOR_PAIR(1));
        mvaddstr(16, 16,"--------------------------------------------");
      	mvaddstr(17, 16, "| #  #   #   #   #  #### #   #   #   #   # |");
      	mvaddstr(18, 16, "| #  #  # #  ##  # #     ## ##  # #  ##  # |");
      	mvaddstr(19, 16, "| #### ##### # # # #  ## # # # ##### # # # |");
      	mvaddstr(20, 16, "| #  # #   # #  ## #   # #   # #   # #  ## |");
      	mvaddstr(21, 16, "| #  # #   # #   #  ###  #   # #   # #   # |");
      	mvaddstr(22, 16, "--------------------------------------------");

        refresh();

        while((c = wgetch(my_menu_win))) 
        {  if(c == 'Q'){
            break;
            //actioneaza cat timp nu se doreste exitul din joc
        }
            switch(c) 
                {    //setez sagetile pentru miscarea in joc
                        case KEY_DOWN: 
                                menu_driver(my_menu, REQ_DOWN_ITEM);        
                                break;        
                        case KEY_UP:        
                                menu_driver(my_menu, REQ_UP_ITEM);        
                                break;      
                 wrefresh(my_menu_win);

                }

            if( c == '\n'){
                //selectez o actiune ulterioara pentru fiecare optiune
                if( "New Game" == item_name(current_item(my_menu)))
                   game(argc, argv);
               //incepe o runda noua de joc
                if( "Quit" == item_name(current_item(my_menu)))
                	break;
                //se iese din meniu, in terminal
                if("About Hangman" == item_name(current_item(my_menu)))
                {
                	clear();
                	history(argc, argv);
                    //noua optiune pusa care ofera istoria + strategii
                }

                else
                    continue;
            }      
              refresh();
        } 

        //eliberez memoria pentru fiecare
        unpost_menu(my_menu); 
        free_menu(my_menu); 
        for(i = 0; i < n_choices; ++i) 
                free_item(my_items[i]); 
        endwin();        
} 

void re_the_big_menu(int argc, char **argv){
    //meniul care permite accesarea optiunii Resume Game

    	int row, col;
        getmaxyx(stdscr, row, col);
        //aflu coordonatele spatiale 
        ITEM **my_items;       
        int c;                                        
        MENU *my_menu;        
        WINDOW *my_menu_win; 
        int n_choices, i; //nr elemente
        //initializez
        initscr(); 
        clear();       
        start_color();        
        cbreak(); 
        noecho(); 
        keypad(stdscr,TRUE);        
        //culorile perechi
        init_pair(1, COLOR_BLACK, COLOR_CYAN); 
        init_pair(2, COLOR_MAGENTA, COLOR_RED);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        //creare item
        n_choices = ARRAY_SIZE(choices); // nr elemente
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *)); 
        //aloca pt nr elemente 
        for(i = 0; i < n_choices; ++i) 
                my_items[i] = new_item(choices[i], choices[i]); 
        //creare meniu

                my_menu = new_menu((ITEM **)my_items); 
        //creez fereastra asociata meniului
        my_menu_win = newwin(10, 40, 4, 4); // a creat fereastra
        keypad(my_menu_win, TRUE); 
        //setez fereastra principala si fereastra secundara
        set_menu_win(my_menu, my_menu_win); 
        bkgd(COLOR_PAIR(2));
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1)); 
        //setez o marca pentru stringul ales
        set_menu_mark(my_menu, " * "); 
        //afisez o bordura in jurul ferestrei principale
        //afisez un titlu pentru meniu
        box(my_menu_win, 0, 0); 
        print_in_middle(my_menu_win, 1, 0, 40, "My Menu", COLOR_PAIR(1));
        mvwaddch(my_menu_win, 2, 0, ACS_LTEE); 
        mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38); 
        //afisez meniul
        post_menu(my_menu);        
        wrefresh(my_menu_win);  

        
        attron(COLOR_PAIR(1));
        //aleg un set de culori pentru titlul jocului
        mvaddstr(16, 16,"--------------------------------------------");
      	mvaddstr(17, 16, "| #  #   #   #   #  #### #   #   #   #   # |");
      	mvaddstr(18, 16, "| #  #  # #  ##  # #     ## ##  # #  ##  # |");
      	mvaddstr(19, 16, "| #### ##### # # # #  ## # # # ##### # # # |");
      	mvaddstr(20, 16, "| #  # #   # #  ## #   # #   # #   # #  ## |");
      	mvaddstr(21, 16, "| #  # #   # #   #  ###  #   # #   # #   # |");
      	mvaddstr(22, 16, "--------------------------------------------");
        //afisez titlul jocului in format mare
        refresh();

        while((c = wgetch(my_menu_win))) 
        {  if(c == 'Q'){
            break;
            //in cazul in care userul doreste sa iasa
        }
            switch(c) 
                {      //sagetile sus, joc pentru meniu
                        case KEY_DOWN: 
                                menu_driver(my_menu, REQ_DOWN_ITEM);        
                                break;        
                        case KEY_UP:        
                                menu_driver(my_menu, REQ_UP_ITEM);        
                                break;      
                 wrefresh(my_menu_win);

                }

            if( c == '\n'){
                if( "New Game" == item_name(current_item(my_menu)))
                   game(argc, argv);
               //inceperea unui joc nou
                if( "Quit" == item_name(current_item(my_menu)))
                	break;
                //iesirea din casuta de meniu
                if("About Hangman" == item_name(current_item(my_menu)))
                {
                	clear();
                	history(argc, argv);
                    //noua optiune cu strategi si istoria jocului
                }
                if("Resume Game" == item_name(current_item(my_menu)))
                {
                	clear();
                    //optiunea este acum activa
                
                }
            }      
              refresh();
        } 

        //eliberez toata memoria
        unpost_menu(my_menu); 
        free_menu(my_menu); 
        for(i = 0; i < n_choices; ++i) 
                free_item(my_items[i]); 
        endwin();        
} 




void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color) 

{       //afisare

        int length, x, y; 
        float temp;        
        if(win == NULL) 
                win = stdscr; 
        getyx(win, y, x); 
        if(startx != 0) 
                x = startx; 
        if(starty != 0) 
                y = starty; 
        if(width == 0) 
        width = 75; 
        length = strlen(string); 
        temp = (width - length)/ 2; 
        x = startx + (int)temp; 
        wattron(win, color);        
        mvwprintw(win, y, x, "%s", string); 
        wattroff(win, color);        
        refresh();        
}

int printabil( char *result){
    //testez daca o expresie este printabila
    //daca aceasta contine cel putin un caracter printabil
	int i = 0, c = 0;
	int lungime;
	lungime = strlen(result);
	while( i <=lungime ){
		if(isprint(result[i]) != 0)
		{
		c++;
		}
		i++;
	}
	
	if( c == 0)
		return 2;
	else
		return 1;
    //minim un caracter printabil garanteaza pentru existenta
    //lungimea expresiei viitoare
}	

char * modificare_printabil( char *result)
{   //modific expresia care poate avea cel putin 
    //un caracter printabil
    int j = 1, k = 0, lungime, i=0, m;
    int lungime_2;
    int l;
    lungime = strlen(result);
    char c;
    char *str;
    char *str2;
    str = (char*)malloc(sizeof(char));
    //aloc memorie si formez un sir din caractere printabile
    //caracterele neprintabile sunt evitate
    while(k <= lungime ){
    if( isprint(result[k]) != 0){
        c = result[k];
        str = (char*)realloc(str, (k+1) * sizeof(char));
        str[i] = c;
        j++;
        i++;
    }
    k++;
}
    l = strlen(str);
    //lungimea sirului printabil obtinut

        while((str[i] != ' ') && (result[i+1] != ' ')){
            //verific daca exista spatii consecutive
        for(k = 0; k < l; k++){
            if(str[k] == ' ' && str[k+1] == ' ')
            {
                for(m = k + 1; m < l; m++)
                    str[m] = str[m+1];
                //elimin spatiile
            }
        }
        i++;
    }
    lungime_2 = strlen(str);

    str2 = (char*)malloc(lungime_2 * (sizeof(char)));
    //formez un nou sir final, fara caractere neprintabile
    //si fara spatii aditionale, inutile
    strcpy(str2, str);

    return str2;
}


int expresii_valide(int argc, char **argv)
{
    //calculez nr de expresii valide din fisiere
    //pot exista expresii care sunt integral neprintabile
    int i = 0, k = 0;
    int l;
    char **words;
    words = (char **)calloc(1, sizeof(char *));
    char *linie = (char *)calloc(1000, sizeof(linie));
    int size;

    for( k = 1; k < argc; k++){
    //verific daca toate fisierele se pot deschide ok
    FILE *fp = fopen(argv[k], "r");
        if (fp == 0){
        fprintf(stderr, "[Eroare]: Fisierul %s nu poate fi deschis.", argv[k]);
        exit(1);
        }
    }

    for( k = 1; k < argc; k++){
    FILE *fp = fopen(argv[k], "r");
        if( fp = fopen(argv[k], "r")) {
    
    while(fgets(linie, 1000, fp) && *linie != EOF){
        linie[strlen(linie)-1] = '\0';
        l = strlen(linie);
    //parcurg toate liniile si verific daca sunt printabile
        if(printabil(linie) == 1){
            words = (char **)realloc(words, (i + 1) * sizeof(char *));
            words[i] = (char *)calloc(1000, sizeof(char));
            strcpy(words[i], linie);
            i++;
        //obtin forma printabila si modificata
        }
        size = i;
        //numar caracterele valide
        }
    fclose(fp);
    }
}

 	return size;
}


	int total_caractere(int argc, char **argv)
	{ //calculez numarul total de caractere

	int i = 0, lungime, total = 0, t, k = 0;
	char **words;
	words = (char **)calloc(1, sizeof(char *));
    //aloc spatiu pentru un vector de siruri
	char *linie = (char *)calloc(1000, sizeof(linie));

    for(k = 1; k < argc; k++){
            FILE *fp = fopen(argv[k], "r");
    if (fp == 0){
        fprintf(stderr, "[Eroare]: Fisierul %s nu poate fi deschis.", argv[k]);
        exit(1);
        }
    } // verific daca fisierul se poate deschide ok

    for(k = 1; k < argc; k++){
        //parcurg liniile din fisier
        FILE *fp = fopen(argv[k], "r");
            if( fp = fopen(argv[k], "r")) {
        while(fgets(linie, 1000, fp) && *linie != EOF){
            linie[strlen(linie)-1] = '\0';
            lungime = strlen(linie);
        //printf("Lungime lui %s este %d.\n", linie, l);
        if(printabil(linie) == 1){
            words = (char **)realloc(words, (i + 1) * sizeof(char *));
            words[i] = (char *)calloc(1000, sizeof(char));
            //printf("este printabil: %s\n", linie);
            strcpy(words[i], linie);
            i++;
            total = total + lungime;
            //incrementez mereu numarul total(+)
            }
        
        }
    fclose(fp);
        }
    }
		return total;
        //returnez numarul total de caractere
}


char *read_from_file(int argc, char **argv)
{ //citesc din fisiere pentru a obtine un element care
    //se supune cerintelor de printabilitate

	int i = 0;
    int k = 1;
	int l;
	char **words;
	words = (char **)calloc(1, sizeof(char *));
    //aloc memorie pentru a le stoca pe toate cele bune
	char *linie = (char *)calloc(1000, sizeof(linie));
	int size;

    for(k = 1; k < argc; k++){
            FILE *fp = fopen(argv[k], "r");
    if (fp == 0){
        fprintf(stderr, "[Eroare]: Fisierul %s nu poate fi deschis.", argv[k]);
        exit(1);
        }
    } //verific daca vreun fisier este imposibil de deschis


    for(k = 1; k < argc; k++){
        FILE *fp = fopen(argv[k], "r");
         if( fp = fopen(argv[k], "r")) {
            //parcurg fisierele date
    
    while(fgets(linie, 1000, fp) && *linie != EOF){
        linie[strlen(linie)-1] = '\0';
        l = strlen(linie);
        if(printabil(linie) == 1){
            //verific daca linia data expresiei este printabila
            words = (char **)realloc(words, k * sizeof(char *));
            words[i] = (char *)calloc(1000, sizeof(char));
            //daca este, realoc spatiu pentru a o stoca 
            int s = sizeof(modificare_printabil(linie))/sizeof(char);
            char *c;
            c = (char *)malloc(s * sizeof(char));
            c = modificare_printabil(linie);
            strcpy(words[i], c);
            //o stochez
            i++;
            k = k + 2;
            //numar 
        }
    }
    
 }
    fclose(fp);
    }
  
 	srand(time(NULL));
 	int j = rand()%i;
 	int m = (j+1)%i;
    //calculez un intem in mod random pentru a genera un cuvant
 
 	return words[j];
    //obtin cuvantul care va fi folosit in joc
}


void game(int argc, char **argv)
{   //functia care se ocupa de miscarea completa a jocului

	int row,col; 
	int c; 
    char final;
	initscr();
	getmaxyx(stdscr,row,col);  
    //obtin coordonatele spatiale 
	clear();
	noecho(); 
	cbreak();         
	start_color();     

	char str[] = "_"; 
	char str1[] = "|"; 
	char str2[] = "="; 
	char str3[] = "*";   
    //caractere care ajuta la construirea spanzuratorii        


	init_pair(1, COLOR_RED, COLOR_GREEN);
 	init_pair(2, COLOR_CYAN, COLOR_RED);
 	init_pair(3, COLOR_BLACK, COLOR_BLUE);
 	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    //initializez niste perechi de culori pe care
    //le folosesc la fundal sau pentru segmente de desen
	
	int numLives = 6;// nr vieti  
	int readCount = 0;
	int correct = 2; //cate litere sunt ok
	int other_correct = 0; //va deveni ulterior correct
	int lengthOfStr;
	int i = 0, j = 0, g = 0, m = 0;
	int lungime;

	int loopIndex = 0;
	int loopSec, loopMeh;
	int key, save, flag;

	//pentru timp de asteptare
	char randChar;
    int randNum = 0;

    //pozitii predefinite pentru aparitii de text
	int pozitie_coloana;
	int pozitie_coloana2 = 35;
    int pozitie_coloana3 = 35;

	char guess; // stochez literele introduse de user

	char letterEntered; //litera de la user

	char *result = read_from_file(argc, argv);
    //obtin cuvantul random 
	int lengthOfWord = strlen(result); 
	//lungime cuvant ales random

		for(j = 0; result[j]; j++){
			result[j] = tolower(result[j]);
            //literele mari -> litere mici
		}

        //ma asigur ca nu mai exista spatii aditionale in cuvant
        while((result[j] != ' ') && (result[j+1] != ' ')){
        for(g = 0; g < lengthOfWord; g++){
            if(result[g] == ' ' && result[g+1] == ' ')
            {
                for(m = g + 1; m < lengthOfWord; m++)
                    result[m] = result[m+1];
            }
        }
        j++;
    }


	
	//fiecare numaru din vector va fi 1 sau 0
    //1 daca s-a ghicit litera si apare in desen
    //0 daca inca nu s-a ghicit si nu apare pe desen
	//int letterGuessed[50] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
     char letterGuessed[300] = {0};
    //prechi de culori
	init_pair(1, COLOR_RED, COLOR_GREEN);
 	init_pair(2, COLOR_CYAN, COLOR_RED);
 	init_pair(3, COLOR_BLACK, COLOR_BLUE);
 	init_pair(4, COLOR_BLACK, COLOR_YELLOW);

    //desen pentru spanzuratoare
 	int q;
	for (q = 4; q<=col-57; q++){
 	   mvprintw(4, q, "%s", str); 
	}
	int s;
	for( s = 4 ; s<=row-4; s++){
    	mvprintw(s, 7, "%s", str1); 
	}
	int v;
	for ( v = 4; v<=row-18; v++){
    	mvprintw(v, 20, "%s", str1);
	}
	int p;
	for( p = 4; p<=col-69; p++){
    	mvprintw(row-5, p, "%s", str2);
	}
	int b;
	for( b = 4; b<=col-69; b++){
   		mvprintw(row-4, b, "%s", str3);
	}
	refresh();

    //afisarea timpului si a datei curente
	time_t t;
	time(&t);
	attron(COLOR_PAIR(1));
	mvprintw(1, 1, "%s",ctime(&t));
    //instructiune din cadrul legendei
	mvprintw(2, 1, "Press Q for exit, any letter to resolve!");

    //calculez numarul de expresii valide
    //folosind functia de mai sus
	int nr_val = expresii_valide(argc, argv);
	attron(COLOR_PAIR(1));
    //folosesc o pereche de culori pentru acest afisaj
	mvprintw(row-3, 1, "Expresii valide:%d", nr_val);
	refresh();

    //calculez numarul total de caractere din fisier
	int total = total_caractere(argc, argv);
    //folosesc o pereche de culori
	attron(COLOR_PAIR(4));
    //fisez numaru total
	mvprintw(row-2, 1, "Total caractere:%d", total);
	refresh(); 

    //creez un vector pentru caracterele introduce gresit
    //ne vom asigrua ca introducerea lor ulterioara, repetata,
    //nu afecteaza jocul, numaru de vieti
    char remember[7];

    //stiu, din prima, ca prima si ultima litera 
    //a expresiei pot fi afisate si apar ele intai
	letterGuessed[0] = 1;
	letterGuessed[lengthOfWord-1] = 1;

	while(correct <= lengthOfWord) {
        //jocul decurge atat timp cat mai sunt litere neghicite

		pozitie_coloana = 22;
	
		for ( loopIndex = 0; loopIndex < lengthOfWord; loopIndex++) {  

				if(letterGuessed[loopIndex] == 1) {
				mvprintw(12, pozitie_coloana,"%c", result[loopIndex]);
				refresh();
                //daca valoarea din vector a itemului e 1, apare litera
                //corespunzatoare itemului in cuvant
			
			}
				else {
					mvaddstr(12, pozitie_coloana, "_");
					refresh();
                    //daca valoarea din vector a itemului e 0, nu apare
                    //litera, dar apare in cuvant "_"
				}

				pozitie_coloana++;
                //creste pozitia standard pentru afisarea cuvantului
		}

		mvprintw(18, 35,"Ai ghicit:%d", correct);
        //afisez numarul de litere ghicite
		refresh();
		mvprintw(19, 35, "Litere introduse deja:");
        //afisez literele introduse in mod curent
		refresh();
	

		if( input() == 1 ){
		  guess = getchar();
        //daca utilizatorul nu a depasit timeout-ul standart de 7s

    	  if( guess == 'Q'){
      	 	 clear();
       	 	 re_the_big_menu(argc, argv);
             //verific daca doreste parasirea jocului
    	}
		else{

		  	letterEntered = guess;
            //daca nu doreste parasirea jocului
            //litera introdusa se ia drept litera propusa in joc
			}
		}  

			
		else{ 
            //utilizatorul nu a introdus o litera in timp util

		while (save = 0) 
		{
            while(letterEntered != 'Q'){
                //cat timp nu se introduce litera de exit
                //putem genera una random 
            srand(time(NULL));
            randNum = (32  + (rand() * 95));
            randNum = randNum + 97;
            randChar = (char)randNum;
            letterEntered = randChar; 
        }
	
        	for(loopIndex = 0; loopIndex < lengthOfWord; loopIndex++){
          		if(result[loopIndex] == letterEntered){
          		key = loopIndex;//indicele
          		flag = 1;// stim ca litera introdusa se gaseste deja
          		break;
          	}
         		else
          			flag = 0;
                //nu am detectat o litera egala cu litera random
          	}//for

          	if(flag == 1)
                //am gasit o litera egala cu cea introdusa
          		{
          			if(letterGuessed[key] == 1){ 
                    //litera propusa a fost deja propusa anterior
          			save = 0;
          		}
          		else
          			{
          				save = 1;//nu 
          			}
          		}
          	else //flag = 0;
          		save = 0;
			}
		}

		time_t t;
    	time(&t);
    	attron(COLOR_PAIR(2));
    	mvprintw(1, 1, "%s",ctime(&t));
        //se actualizeaza data si ora in panoul de control

        //valoare de correct se preia
		other_correct = correct;

        int found = 0;
        int w = 0;

		for ( loopIndex = 0; loopIndex < lengthOfWord; loopIndex++ ) {

			if(letterGuessed[loopIndex] == 1){
				continue;
            //vectorul letterGuessed anunta ca litera e pe pozitie 1
            //adica a fost ghicita anterior
			}

			if(letterEntered == result[loopIndex]){
            //verific daca litera propusa se gaseste in cuvant
				letterGuessed[loopIndex] = 1;
            //ii schimb valorea din vector si incrementez
				correct++;
                found = 1;

			}


			if((result[loopIndex] == result[0]) || (result[loopIndex] == result[lengthOfWord-1])){
            //verific daca exista litere in cuvant egale cu
                //prima sau ultima litera
					if( letterGuessed[loopIndex] == 0 ){
						letterGuessed[loopIndex] = 1;
						correct++;
                //doar in cazul in care sunt pe pozitie 0, le schimb in 1
                //ulterior vor fi afisate
					}
					else
						continue;
					
				}

				if(result[loopIndex] == ' '){
                //verific daca spatiul are caracteristici speciale

                if(result[loopIndex-2] == ' '){
                    letterGuessed[loopIndex-1] = 0;
                //daca literele de pe pozitii -2 anterioare sunt pe 0
                }
                else
                            if(letterGuessed[loopIndex-1] == 0){
                    //doar daca pozitiile -1 sunt pe 0, modific
                    letterGuessed[loopIndex-1] = 1;
                    correct++;
                    for(loopMeh = 0; loopMeh < lengthOfWord; loopMeh ++){
                    //verific daca litera analizata mai apare in alta parte
                        if(result[loopMeh] == result[loopIndex-1]){
                            if(letterGuessed[loopMeh] == 0){
                    //doar in cazul in care e pe 0, devine 1
                                letterGuessed[loopMeh] = 1;
                                correct++;
                            }
                        }
                    }
                }


	
				
				if(letterGuessed[loopIndex] == 0){
					letterGuessed[loopIndex] = 1;
					correct++;
                //daca spatiul insusi inca nu e detectat, il trec pe 1
				}



			     if(result[loopIndex+2] == ' '){
                    letterGuessed[loopIndex+1] = 0;
                //analizez pozitiile +2 ale spatiului
                 }
                 else
                    if(letterGuessed[loopIndex+1] == 0){
                    //doar daca pozitiile +1 sunt pe 0, le fac 1
                    letterGuessed[loopIndex+1] = 1;
                    correct++;
                    for(loopMeh = 0; loopMeh < lengthOfWord; loopMeh ++){
                    //verifica daca litera de pe pozitia +1 se mai gaseste
                        if(result[loopMeh] == result[loopIndex+1]){
                            if(letterGuessed[loopMeh] == 0){
                            //doar daca e pe 0, o trec pe 1
                                letterGuessed[loopMeh] = 1;
                                correct++;
                            }
                        }
                    }
                }
			}
	   }

        if(found == 0){
        //daca litera nu s-a gasit, o bag intr-un vector special
        remember[w] = letterEntered;
        remember[w+1] = '\0';
        w++;
            //afisez litera aleasa
            mvprintw(20, pozitie_coloana3, "%c", letterEntered);
            pozitie_coloana3++;
            refresh();
        }   

        if (other_correct == correct){
            //nu s-a ghicit litera;
            //nu a crescut correct;
            numLives = numLives-1;
            } 

        //in functie de numarul de vieti ramase
        //stim ce parte a corpului trebuie desenata

		if(numLives == 5){
        //desenez capul pentru 5 vieti

			attron(COLOR_PAIR(3));
			mvaddstr(7, 19, "*");
			mvaddstr(7, 20, "*");
			mvaddstr(7, 21, "*");
			mvaddstr(8, 18, "*");
			mvaddstr(8, 19, "*");
			mvaddstr(8, 20, "*");
			mvaddstr(8, 21, "*");
			mvaddstr(8, 22, "*");
			mvaddstr(9, 19, "*");
			mvaddstr(9, 20, "*");
			mvaddstr(9, 21, "*");
		}
		refresh();

		if(numLives == 4){
        //desenezz gatul pentru 4 vieti

			attron(COLOR_PAIR(2));
			mvaddstr(10, 20, "|");
			mvaddstr(11, 20, "|");
		}
		refresh();

		if(numLives == 3){
        //desenez o mana pentru 3 vieti

			attron(COLOR_PAIR(1));
			mvaddstr(12, 19, "/");
			mvaddstr(13, 18, "/");
		}
		refresh();

		if(numLives == 2){
        //desenez cealalta mana pentru 2 vieti

			attron(COLOR_PAIR(3));
			mvaddstr(11, 21, ">");
			mvaddstr(11, 22, ">");
			mvaddstr(11, 23, ">");
		}
		refresh();

		if(numLives == 1){
        //desenez trunchi pentru 1 viata

			attron(COLOR_PAIR(1));
			mvaddstr(12, 20, "|");
			mvaddstr(13, 20, "|");
		}
		refresh();

		if(numLives == 0){
        //desenez picioarele pentru faza finala

			attron(COLOR_PAIR(2));
			mvaddstr(14, 19, "*");
			mvaddstr(15, 18, "*");
			mvaddstr(16, 17, "*");
			mvaddstr(14, 21, "*");
			mvaddstr(15, 22, "*");
			mvaddstr(16, 23, "*");
			attron(COLOR_PAIR(4));

            //afisez mesaj corespunzator pentru pierzator
			mvprintw(16, 35, "You lose! :( Word was: %s", result);
		}
		refresh();

		  if(correct == lengthOfWord){
        //daca s-a castigat runda
                clear();
                noecho();
                cbreak();
                attron(COLOR_PAIR(3));
                //afisez intr-un nou ecran un mesaj pentru castigator
				mvaddstr(15, 35, "YOU'RE THE BEST, NO HANG FOR TODAY!");
                mvaddstr(16, 35, "*** ^_^ ^_^ ^_^ ***");
                //afisez comenzi ajutatoare
                mvaddstr(9, 30, "Press N for a new game!");
                mvaddstr(10, 30, "Press Q for exit!");
                refresh();

                final = getchar();
                if( final == 'Q'){
                //din faza finala se poate alege intoarcerea la meniu
                    clear();
                    the_big_menu(argc, argv);
                    refresh();
                }
                if( final == 'N'){
                //din faza finala se alege reinceperea unui joc nou
                    clear();
                    game(argc, argv);
                    refresh();
                }
			}

		}
		//while loop

	endwin();
	}

	int main(int argc, char **argv){
        //verific daca s-au dat argumente suficiente 
			if(argc == 1){
				fputs("[Eroare]: Nu s-au dat argumente de comanda.", stderr);
			}
			else
			{    
				the_big_menu(argc, argv);
			}
			

		return 1;
	}
