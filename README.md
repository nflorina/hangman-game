# hangman-game
Hangman is a guessing game for two or more players. One player thinks of a word, phrase or sentence and the other tries to guess it by suggesting letters or numbers, within a certain number of guesses.

Mine is implemented using ncurses.

  Totul tine in mod aprioric de functia main in cadrul careia sunt specificate cu precizie functiile a caror executare duce la rezolvarea taskurilor dupa cum urmeaz

  Functia 'the_big_menu' afiseaza meniul principal de unde utilizatorul poate alege una din cele 3 optiuni puse la dispozitie: New Game(prin apelarea functiei 'game'), Quit( pentru parasirea meniului si revenirea la terminal ), Resume Game( care nu este inca accesibila in faza aceasta). S-a adaugat optiunea About Hangman( functia 'History') care ofera informatii utile despre istoria jocului si despre strategii de baza.
  Algerile se fac prin sagetile sus/jos urmate de Enter.
  S-a adaugat numele jocului in centrul ecranului prin functii de afisare.
  
  Functia 'game' contine si referiri la layot. S-a desenat spanzuratoarea, s-a afisat pe ecran data si ora, care se vor actualiza. In plus, exista o legenda care permite userului sa aleaga din comenzile puse la dispozitie: se apasa o litera pentru a contribui la joc, iar Q este special destinata exit-ului.
  
  In cadrul functiei 'main' se verifica daca aplicatia ruleaza cu argumente. In caz contrar, primeste eroare al iesire. Functia 'read_from_file' alege, dupa parcurgerea tuturor liniilor din toate fisierele, cuvantul potrivit. Se verifica prin functia 'printabil' daca acesta contine cel putin un caracter printabil pentru a fi posibila prelucarea lui si este, ulterior, stocat intr-un vector alocat dinamic. In cazul in care nu contine niciun caracter printabil, linia e evitata.
	Functia 'modificare_printabil' sterge toate caracterele neprintabile din cuvant si se obtine un cuvant total printabil. Ulterior, sunt eliminate spatiile in plus. In functiile 'read_from_file' si'modificare_printabil' se verifica daca exista vreun fisier imposibild e deschis.
	Functiile 'expresii_valide' si 'total_caractere' parcurg fisierele in intregime si evalueaza numarul de caractere si expresii valide pentru a le afisa in joc.
  
  
  S-a ales expresia prin functia 'read_from_file'. Ea va fi transmisa in functia 'game' drept cuvant tinta. Sirul ascuns se genereaza dupa cum urmeaza:
	* apar intai ultima si prima litera, dupa o apasare de tasta penru a ghici cuvantul, apare litera, daca a fost ghicita, dar si restul caracterelor din sirul ascuns. Functia 'game' analizeaza prezenta literelor in jurul spatiilor pentru o buna aparitie a sirului ascuns.
	In functia 'game' exista un vector format din 0 si 1, letterGuessed.
	Variabila 'correct' va stoca numarul de litere care au fost ghicite si in functie de avansarea in acest numar, se va lua decizia de spanzuratoare sau nu. Daca nu se avanseaza in numar, se scade o viata. Pentru fiecare numar de vieti, se ia o decizie si se deseneaza o parte din corp.
	Daca s-au completat corect toate casutele, atunci apare un ecran nou cu un mesaj de felicitare.
  
  Functia 'input' verifica daca in timpul maxim oferit s-a introdus sau nu ceva.
	In primul caz, se ia ca atare litera spre analiza in joc, in al doilea caz, se va genera random un caracter printabil. Se va verifica sa nu fie un caracter deja introdus si aflat in cuvant.
  
