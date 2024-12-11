
from queue import Empty
import random


def vytvor_matici(n: int, m: int) -> list[list[int]]:
    """Vytvoří matici n x m s náhodnými celými čísly od 0 do 9."""
    matice: list[list[int]] = []
    matice = [[random.randint(0,9) for col in range(m)] for row in range(n)] # vytvoření listu v listu s náhodnýmy inty
    return matice


def reprezentace_matice(matice: list[list[int]]) -> str:
    """Vrátí stringovou reprezentaci matice."""
    rem = ""
    for row in matice:
        for item in row:
            rem += str(item) + " " # jednotlivé položky v řádku
        
        rem = rem[:-1] # odstranění přebytečné mezery, negativní indexování -> poslední prvek stringu
        rem += "\n" # odsazeni
            
    return rem


def soucet_matic(matice1: list[list[int]], matice2: list[list[int]]) -> list[list[int]]:
    """Sečte dvě matice, pokud mají stejné rozměry."""
    m = len(matice1[0])
    n = len(matice2[0])
    if m != n or len(matice1) != len(matice2):
        return None
    
    matice: list[list[int]] = []
    matice = [[matice1[row][col] + matice2[row][col] for col in range(m)] for row in range(len(matice1))] # sečtení na totožných indexech pokud projde checkem za pomocí zkráceného syntaxu
    return matice


def nasobeni_matic(matice1: list[list[int]], matice2: list[list[int]]) -> list[list[int]]:
    """Vynásobí dvě matice, pokud je násobení proveditelné."""
    if len(matice1[0]) == 0 and len(matice2[0]) == 0:
        return []
    if len(matice1[0]) != len(matice2): # check na velikost sloupce první mat a řádků druhé mat
        return None
    matice: list[list[int]] = []
    # for loopy pro lepší přehlednost -> přetvořeno na zkrácený syntax 
    """for i in range(len(matice1)): # iterace řadku od m1
        for j in range(len(matice2[0])): # iterace sloupce m2
            for k in range(len(matice2)): # iterace řádky m2
                matice[i][k] += (matice1[i][k] * matice2[k][j])
    """
    matice = [[sum(matice1[i][k] * matice2[k][j] for k in range(len(matice2))) for j in range(len(matice2[0]))] for i in range(len(matice1))]
    return matice


def transpozice_matice(matice: list[list[int]]) -> list[list[int]]:
    """Provede transpozici matice."""
    matice: list[list[int]] = []
    if len(matice) == 0:
        return [[]] if matice else []
        
 
    matice = [[matice[col][row] for col in range(len(matice))] for row in range(len(matice[0]))] # prohození řádků se sloupci
    return matice


if __name__ == "__main__":
    matice1: list[list[int]] = vytvor_matici(3, 2)
    matice2: list[list[int]] = vytvor_matici(2, 4)

    print("Matice 1:")
    print(reprezentace_matice(matice1))
    print("Matice 2:")
    print(reprezentace_matice(matice2))

    soucet = soucet_matic(matice1, matice1)  # Sečteme matici1 samu se sebou
    print("Součet matic:")
    print(reprezentace_matice(soucet))

    nasobek = nasobeni_matic(matice1, matice2)
    print("Násobení matic:")
    print(reprezentace_matice(nasobek))

    transponovana = transpozice_matice(matice1)
    print("Transponovaná matice:")
    print(reprezentace_matice(transponovana))
