# ft_irc

## Description  
**ft_irc** est une implémentation d’un serveur IRC (Internet Relay Chat) développé en C++. Ce projet a pour objectif d’approfondir la compréhension des protocoles réseau, de la gestion des connexions multiples et de la communication en temps réel entre clients.  

Le projet inclut également un **bot IRC** capable de jouer à un jeu de shifumi (pierre-papier-ciseaux) avec les utilisateurs.

## Fonctionnalités  
- Gestion de multiples connexions clients via des sockets.  
- Implémentation des commandes principales du protocole IRC (JOIN, PART, PRIVMSG, etc.).  
- Support des canaux publics et privés.  
- Authentification des utilisateurs avec gestion des permissions et modes.  
- **Bot IRC** :  
  - Se connecte au serveur et crée un canal à son nom.  
  - Lorsqu'un utilisateur envoie la commande **!play**, le bot répond par un message privé avec un choix aléatoire parmi **rock**, **scissor**, ou **paper**.  
  - Le bot peut également être invité dans un canal pour jouer directement avec les utilisateurs présents dans ce canal.  
  - Les utilisateurs peuvent rejoindre le canal du bot pour participer au jeu.  

## Usage  
Pour lancer le serveur :  

    ./ircserv <port> <password>

    <port> : Port sur lequel le serveur écoutera (ex. : 8080).
    <password> : Mot de passe requis pour que les clients puissent se connecter.

Pour tester, nous recommandons le client IRC HexChat. Configurez-le avec l’adresse et le port de votre serveur pour vous connecter.
Commandes implémentées

    /NICK : Définit ou modifie le pseudonyme d’un utilisateur.
    /USER : Identifie un utilisateur auprès du serveur.
    /JOIN : Rejoint un canal spécifique.
    /PART : Quitte un canal.
    /PRIVMSG : Envoie un message privé à un utilisateur ou canal.
    /QUIT : Déconnecte un utilisateur du serveur.
    /KICK : Éjecte un client du canal.
    /INVITE : Invite un client dans un canal.
    /TOPIC : Change ou affiche le sujet d’un canal.
    /MODE : Change le mode du canal (voir options ci-dessous) :
        i : Définit/enlève le mode "canal réservé aux invitations".
        t : Définit/enlève les restrictions sur les utilisateurs pouvant changer le sujet du canal (réservé aux opérateurs).
        k : Définit/enlève le mot de passe du canal (clé).
        o : Attribue/retire les privilèges d'opérateur du canal.
        l : Définit/enlève la limite d’utilisateurs dans un canal.

Structure du projet

    manda/ : Contient le code principal pour l’implémentation de base du serveur IRC.
    bonus/ : Contient le code du bot IRC pour jouer à shifumi.
    Makefile : Automatisation de la compilation.
    README.md : Documentation du projet.

Améliorations possibles

    Intégration de SSL/TLS pour sécuriser les échanges.
    Implémentation de commandes supplémentaires (banissement, etc.).
    Gestion des logs serveur pour un suivi plus détaillé.
    Ajout de limites de débit pour prévenir les abus.

Auteurs

Ce projet a été réalisé dans le cadre de la formation à l’École 42 par :

    Quentin Devianne - TonUpMan
    Benoit Darenne - bedarenn
