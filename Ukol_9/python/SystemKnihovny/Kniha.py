class Kniha:
    def __init__(self, nazev: str, autor: str, rok_vydani: int, isbn: str):
        self._nazev = nazev
        self._autor = autor
        self._rok_vydani = rok_vydani
        self.isbn = isbn

    # doplňte potřebné gettry a setry


    @property # dekorátor , info v ctenar.py
    def nazev(self) -> str:
        return self._nazev

    @property
    def autor(self) -> str:
        return self._autor

    @property
    def rok_vydani(self) -> int:
        return self._rok_vydani

    @property
    def isbn(self) -> str:
        return self._isbn

    @isbn.setter
    def isbn(self, isbn: str):
        if len(isbn) == 13 and isbn.isdigit():
            self._isbn = isbn
        else:
            raise ValueError("ISBN must have 13 digits and can not contain any other characters! ")

    def __str__(self) -> str:
        return f"{self.nazev} od {self.autor} ({self.rok_vydani}) [ISBN: {self.isbn}]"
