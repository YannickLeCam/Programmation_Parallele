# Programmation Parallèle

Ce dépôt contient une série de travaux pratiques visant à enseigner les concepts de la programmation parallèle en utilisant le langage C. Les exercices couvrent divers aspects de la parallélisation, de la synchronisation et de la gestion des tâches.

## Contenu du dépôt

### **tp1_openmp_codes** :

Ce dossier contient des exercices et des exemples utilisant OpenMP, une API pour la programmation parallèle en mémoire partagée.

  - Introduction à OpenMP : Comment compiler et exécuter un programme avec OpenMP.

  - Parallélisation des boucles : Utilisation de #pragma omp parallel for pour accélérer l'exécution des boucles.

  - Gestion des threads : Création et gestion dynamique du nombre de threads.

  - Sections parallèles : Distribution du travail entre plusieurs threads avec #pragma omp sections.

  - Synchronisation et réduction : Mécanismes pour éviter les conflits d'accès aux variables partagées et agréger des résultats.

### **tp2_parallelisation_codes** :

Ce dossier se concentre sur les techniques avancées de parallélisation et d'optimisation.

  - Décomposition de tâches : Stratégies pour diviser un problème en sous-problèmes parallèles.

  - Optimisation du partage des ressources : Minimisation des conflits d'accès mémoire et gestion efficace du cache.

  - Exécution asynchrone : Introduction à l'exécution en mode asynchrone pour maximiser l'utilisation du CPU.

  - Benchmarking et analyse de performance : Techniques pour mesurer l'accélération et identifier les goulots d'étranglement.  


### **tp3_synchronisation_et_taches_codes** : 

Ce dossier couvre les mécanismes de synchronisation et de gestion des tâches parallèles.

  - Mutex et verrous : Utilisation des mutex pour assurer l'accès sécurisé aux ressources partagées.

  - Barrières de synchronisation : Coordination de l'exécution des threads à des points précis.

  - Producteur-consommateur : Implémentation de ce modèle classique en programmation parallèle.

  - Gestion des threads avec pthreads : Introduction à la bibliothèque POSIX threads (pthreads) et ses primitives de gestion de threads.


