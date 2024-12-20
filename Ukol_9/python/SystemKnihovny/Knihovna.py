from __future__ import annotations
from .Kniha import Kniha
from .Ctenar import Ctenar
import csv
import datetime


class Knihovna:
    def __init__(self, nazev: str):
        self.nazev = nazev
        self.knihy: list[Kniha] = []
        self.ctenari: list[Ctenar] = []
        self.vypujcene_knihy = {}

    def kniha_existuje(funkce):
        """
        #Dekorátor kontrolující existenci knihy v knihovně.

        Args:
            funkce: Funkce, která má být volána po kontrole existence knihy.
        """

        def wrapper(self, isbn: str, *args, **kwargs):
        #Wrapper funkce kontrolující existenci knihy před voláním dané funkce.
            if not any(kniha.isbn == isbn for kniha in self.knihy):
                raise ValueError("Book with ISBN: " + isbn + " not found! ")
            return funkce(self, isbn, *args, **kwargs)
        return wrapper

    @classmethod
    def z_csv(cls, soubor: str) -> Knihovna:
        """
        Načte data knihovny ze souboru CSV.

        Args:
            soubor: Cesta k souboru CSV.
        Returns:
            Objekt Knihovna načtený ze souboru.
        """

        with open(soubor, mode = 'r') as file:
            reader = csv.DictReader(file)
            namefixReader = csv.reader(file)
            fstr = next(namefixReader)
            name = fstr[0].split(':')[1].strip()
            Knihovna = cls(name)
            for row in reader:
                if row.fieldnames == "kniha": 
                    Knihovna.pridej_knihu(Kniha(row[1], row[2], int(row[3]), row[4]))
                elif row.fieldnames == "ctenar":
                    Knihovna.registruj_ctenare(Ctenar(row[5], row[6]))
        return Knihovna

    def pridej_knihu(self, kniha: Kniha):
        """
        Přidá knihu do knihovny.

        Args:
            kniha: Objekt knihy, který má být přidán.
        """
        self.knihy.append(Kniha)
        pass

    @kniha_existuje
    def odeber_knihu(self, isbn: str):
        """
        Odebere knihu z knihovny.

        Args:
            isbn: ISBN knihy, která má být odebrána.
        Raises:
            ValueError: Pokud kniha s daným ISBN neexistuje.
        """
        for book in self.knihy :
            if book.isbn == isbn :
                self.knihy.remove(book)
                pass
        raise ValueError("Book with ISBN: " + isbn + " not found! ")

    def vyhledej_knihu(self, klicova_slovo: str = "", isbn: str = ""):
        """
        Vyhledá knihy podle klíčového slova nebo ISBN.

        Args:
            klicova_slovo: Klíčové slovo pro vyhledávání v názvu nebo autorovi.
            isbn: ISBN knihy.
        Returns:
            Seznam nalezených knih.
        """
        if isbn:
            return [book for book in self.knihy if book.isbn == isbn]
        return [book for book in self.knihy if klicova_slovo.lower() in book.nazev.lower() or klicova_slovo.lower() in book.autor.lower()]

    def registruj_ctenare(self, ctenar: Ctenar):
        """
        Zaregistruje čtenáře do knihovny.

        Args:
            ctenar: Objekt čtenáře, který má být zaregistrován.
        """
        self.ctenari.append(Ctenar)

    def zrus_registraci_ctenare(self, ctenar: Ctenar):
        """
        Zruší registraci čtenáře v knihovně.

        Args:
            ctenar: Objekt čtenáře, jehož registrace má být zrušena.
        """
        self.ctenari.remove(Ctenar)
        

    def vyhledej_ctenare(self, klicova_slovo: str = "", cislo_prukazky: int = None):
        """
        Vyhledá čtenáře podle klíčového slova nebo čísla průkazky.

        Args:
            klicova_slovo: Klíčové slovo pro vyhledávání v jméně nebo příjmení.
            cislo_prukazky: Číslo průkazky čtenáře.
        Returns:
            Seznam nalezených čtenářů.
        """
        if cislo_prukazky is not None:
            return [registeredUser for registeredUser in self.ctenari if registeredUser.cislo_prukazky == cislo_prukazky]
        return [registeredUser for registeredUser in self.ctenari if klicova_slovo.lower() in registeredUser.jmeno.lower() or klicova_slovo.lower() in registeredUser.prijmeni.lower()]

    @kniha_existuje
    def vypujc_knihu(self, isbn: str, ctenar: Ctenar):
        """
        Vypůjčí knihu čtenáři.

        Args:
            isbn: ISBN knihy, která má být vypůjčena.
            ctenar: Objekt čtenáře, který si knihu půjčuje.
        Raises:
            ValueError: Pokud kniha s daným ISBN neexistuje nebo je již vypůjčena.
        """
        #isbn raise value error už existuje v dekorátoru 
        if isbn in self.vypujcene_knihy:
            raise ValueError("Book is not available! ")
        self.vypujcene_knihy[isbn] = (ctenar, datetime.date.today())
        





    @kniha_existuje
    def vrat_knihu(self, isbn: str, ctenar: Ctenar):
        """
        Vrátí knihu.

        Args:
            isbn: ISBN knihy, která má být vrácena.
            ctenar: Objekt čtenáře, který knihu vrací.
        Raises:
            ValueError: Pokud kniha s daným ISBN není vypůjčena tímto čtenářem.
        """
        if isbn not in self.vypujcene_knihy or self.vypujcene_knihy[isbn][0] != ctenar: # check pro isbn / majitele vypujcene knihy
            raise ValueError("Book with ISBN: " + isbn + " is not lent by you!")
        del self.vypujcene_knihy[isbn] # vymazání checkeru

    def __str__(self) -> str:
        books = ", ".join(book.nazev for book in self.knihy) # str knih
        registeredUsers =  ", ".join(f"{regUser.jmeno} {regUser.prijmeni}" for regUser in self.ctenari) # str ctenaru
        return f"Knihovna {self.nazev}\nKnihy: {books}\nČtenáři: {registeredUsers}"
