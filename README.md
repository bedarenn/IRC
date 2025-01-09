# ft_irc

## Description  
**ft_irc** est une implémentation d’un serveur IRC (Internet Relay Chat) développé en C++. Ce projet a pour but de comprendre les bases des protocoles réseau, de la gestion des connexions multiples et des échanges entre clients dans un environnement multi-utilisateur.  

## Fonctionnalités  
- Gestion de multiples connexions client via des sockets.  
- Implémentation du protocole IRC, y compris les commandes principales (JOIN, PART, PRIVMSG, etc.).  
- Support des canaux de discussion publics et privés.  
- Gestion des utilisateurs : authentification, permissions, et modes utilisateur/canal.  
- Échange en temps réel entre clients connectés.  
- Robustesse face aux déconnexions ou messages invalides.  

## Usage  
Pour lancer le serveur :  
bash
./ircserv <port> <password>

    <port> : Port sur lequel le serveur écoutera (ex. : 6667).
    <password> : Mot de passe que les clients devront fournir pour se connecter.

Pour tester, nous recommandons d’utiliser le client IRC HexChat, disponible sur HexChat. Configurez-le pour vous connecter à l’adresse et au port du serveur lancé.
Commandes implémentées

    /NICK : Définit ou modifie le pseudonyme d’un utilisateur.
    /USER : Identifie un utilisateur auprès du serveur.
    /JOIN : Rejoint un canal spécifique.
    /PART : Quitte un canal.
    /PRIVMSG : Envoie un message privé à un utilisateur ou canal.
    /QUIT : Déconnecte un utilisateur du serveur.

Structure du projet

    src/ : Code source du serveur.
    includes/ : Fichiers d’en-tête.
    Makefile : Automatisation de la compilation.

Améliorations possibles

    Support de SSL/TLS pour sécuriser les échanges.
    Implémentation de commandes IRC supplémentaires (TOPIC, INVITE, KICK, etc.).
    Gestion des logs serveur.
    Limitation des débits pour éviter les abus.

Auteurs

Ce projet a été réalisé dans le cadre de la formation à l’École 42 par :

    Quentin Devianne - TonUpMan
    Benoit Darenne - bedarenn
