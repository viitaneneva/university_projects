# SNAKE - The Revengeance
------------------------------------

Tehty itsenäisesti koulun kurssityönä keväällä 2019.

------------------------------------
## Kuvakaappaus pelistä

![snake](https://user-images.githubusercontent.com/49073599/211890156-d0e69d5d-b2a6-4970-92c9-d6a03f0485f8.png)

---

## Säännöt
Mato kasvaa syömällä pelilaudalle ilmestyviä
violetteja palleroita. Pelin tavoitteena on täyttää
koko pelilauta madon ruumiilla. 

## Pelin päättyminen
Mato kuolee, mikäli se osuu omaan ruumiiseensa,
jos se osuu pelialueen seinämiin tai jos koko pelialue
täyttyy sen ruumiista (=voitto). Näin sattuessa
pelin voi aloittaa alusta. Huomaa kuitenkin, että
tämä nollaa pisteet sekä madon koon. Pelin päättyessä
tapahtuma kirjataan joko voitoksi tai kuolemaksi.

## Ohjaus
Matoa liikutetaan seuraavasti:
[W] - mato liikkuu ylöspäin.
[A] - mato liikkuu vasemmalle.
[S] - mato liikkuu alaspäin.
[D] - mato liikkuu oikealle.

Huom! Mato ei pysty tekemään 180 asteen käännöstä.

## Pelin hetkellinen keskeytys
Painaessasi..
[P] - peli pysähtyy. Käynnistääksesi pelin uudelleen,
      paina näppäintä uudestaan. 
## Muuta
Pelin edetessä vaikeustaso kasvaa. Tämä ilmoitetaan
pelin ylälaidassa tekstillä LEVEL X. Mitä suurempi
X on, sitä nopeampi madon nopeus on. Peliin on
laitettu yhteensä 5 vaikeustasoa. (Tasot vaihtuvat
tässä versiossa nopeasti, jotta testaus on helpompaa)

Mitä enemmän mato syö palleroita, sitä tummempi sen häntä
on. Mato saa siis pelin edetessä hienon liukuvärin.

Tähän mennessä paras tulos tallennetaan pelialueen ylälaitaan,
kohtaan HIGHSCORE. Rikkoaksesi tuloksen tulee sinun saada
enemmän pisteitä, kuin mitä highscore näyttää.
