# CPP05 expliqué pour débutant·e

Ce document explique, exercice par exercice, **comment marche le code** et **pourquoi chaque fonction existe**. L'idée directrice du module CPP05 est simple : simuler une administration bureaucratique avec des **Bureaucrats** (des employés) qui signent et exécutent des **Forms** (des formulaires). Au passage, le module vous fait manipuler deux notions clés du C++ : les **exceptions** et l'**héritage/polymorphisme**.

---

## Ex00 — La classe `Bureaucrat`

### L'idée
Un `Bureaucrat` représente un employé avec un nom et un grade (1 = le plus haut grade, 150 = le plus bas). C'est la brique de base sur laquelle tout le reste du module s'appuie.

### Pourquoi le nom et le grade sont-ils `const` / vérifiés ?
```cpp
std::string const name;
int grade;
```
Le `name` est déclaré `const` : une fois qu'un Bureaucrat s'appelle "Bob", il ne peut plus changer de nom pendant toute sa vie (son "scope"). C'est une décision de conception : dans la vraie vie, on peut renommer une variable, mais on ne veut pas qu'un objet change d'identité en cours de route.

Le grade, lui, n'est **pas** const, car on doit pouvoir le faire monter ou descendre (`incrementGrade`/`decrementGrade`). Mais il doit toujours rester entre 1 et 150. C'est pour ça que le constructeur vérifie :
```cpp
Bureaucrat::Bureaucrat(std::string const &name, int grade) : name(name), grade(grade)
{
    if (this->grade < 1)
        throw Bureaucrat::GradeTooHighException();
    if (this->grade > 150)
        throw Bureaucrat::GradeTooLowException();
}
```
Si on essaie de créer un Bureaucrat avec un grade de 0 (donc "meilleur" que le meilleur grade possible), ça n'a pas de sens : on **lance une exception** au lieu de créer un objet invalide. C'est le principe des exceptions : plutôt que de laisser un objet exister dans un état incohérent, on interrompt immédiatement l'exécution et on "remonte" l'erreur à qui a appelé le code, pour qu'il puisse réagir (avec un `try`/`catch`).

### Pourquoi des classes d'exception imbriquées ?
```cpp
class GradeTooHighException : public std::exception
{
public:
    virtual const char *what() const throw();
};
```
`GradeTooHighException` **hérite** de `std::exception`. Ça veut dire qu'elle "est un type" de `std::exception`, donc n'importe quel code qui sait attraper des `std::exception` (comme `catch (std::exception const &e)`) saura aussi attraper cette exception précise, sans avoir besoin de connaître son nom exact. La fonction `what()` sert à donner un message d'erreur lisible.

### `incrementGrade` / `decrementGrade` : pourquoi ce sens qui semble "inversé" ?
```cpp
void Bureaucrat::incrementGrade()
{
    if (this->grade <= 1)
        throw Bureaucrat::GradeTooHighException();
    this->grade--;
}
```
Comme 1 est le meilleur grade, "augmenter" son grade (= s'améliorer) signifie **diminuer** le nombre. C'est un peu contre-intuitif au premier abord, mais c'est exactement ce que demande le sujet : passer de grade 3 à grade 2 est bien une amélioration ("increment"). Si le Bureaucrat est déjà au grade 1, il ne peut plus s'améliorer : on lance l'exception.

### L'opérateur `<<`
```cpp
std::ostream &operator<<(std::ostream &out, Bureaucrat const &bureaucrat);
```
Cette fonction n'est **pas** une méthode de la classe (pas de `Bureaucrat::`), c'est une fonction libre. Pourquoi ? Parce que `std::cout << bob` s'écrit avec `std::cout` (un `ostream`) à gauche et l'objet à droite. Si c'était une méthode de `Bureaucrat`, il aurait fallu écrire `bob << std::cout`, ce qui est illisible. En définissant l'opérateur en dehors de la classe, on garde la syntaxe naturelle `cout << objet`.

---

## Ex01 — La classe `Form`

### L'idée
Un `Form` (formulaire) a besoin d'être **signé** par un Bureaucrat avant de pouvoir être exécuté plus tard (ex02). Chaque formulaire a un grade minimum requis pour le signer.

### Pourquoi une variable `signedStatus` qui commence à `false` ?
```cpp
Form::Form(...) : ..., signedStatus(false), ...
```
Un formulaire vierge n'est jamais signé au départ — logique. C'est seulement l'appel à `beSigned()` qui peut faire passer ce booléen à `true`, et seulement si les conditions sont réunies.

### `beSigned()` : où se passe la vérification métier
```cpp
void Form::beSigned(Bureaucrat const &bureaucrat)
{
    if (bureaucrat.getGrade() > this->signGrade)
        throw Form::GradeTooLowException();
    this->signedStatus = true;
}
```
Rappel : un grade **plus petit** = un grade **plus élevé**. Donc si `bureaucrat.getGrade() > this->signGrade`, ça veut dire que le Bureaucrat a un grade numériquement plus grand que ce qui est exigé, donc qu'il est **moins gradé** que nécessaire → pas le droit de signer, on lance l'exception.

### Pourquoi `signForm()` est dans `Bureaucrat` et pas dans `Form` ?
```cpp
void Bureaucrat::signForm(Form &form) const
{
    try
    {
        form.beSigned(*this);
        std::cout << this->name << " signed " << form.getName() << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cout << this->name << " couldn't sign " << form.getName()
                   << " because " << e.what() << std::endl;
    }
}
```
C'est le Bureaucrat qui **initie** l'action ("je veux signer ce formulaire"), donc c'est naturel que ce soit une méthode de `Bureaucrat`. Cette méthode appelle `form.beSigned()`, qui fait la vraie vérification, puis **attrape l'exception elle-même** avec un `try`/`catch` pour transformer une erreur technique en message humain ("Bob couldn't sign Form B because Form grade too low"). C'est une bonne pratique : l'exception sert à signaler un problème, mais à un moment donné, il faut la "gérer" plutôt que de la laisser remonter indéfiniment et planter le programme.

---

## Ex02 — `AForm` et les formulaires concrets

### Pourquoi `Form` devient `AForm` (classe abstraite) ?
Le "A" veut dire **Abstract**. Une classe abstraite est une classe qu'on ne peut **jamais instancier directement** — elle sert uniquement de "moule" commun pour d'autres classes qui vont en hériter. On la rend abstraite en lui donnant au moins une **fonction virtuelle pure** :
```cpp
virtual void execute(Bureaucrat const &executor) const = 0;
```
Le `= 0` à la fin signifie "cette fonction n'a pas de corps ici, chaque classe fille est **obligée** de fournir sa propre implémentation". Ça a du sens : "exécuter" un formulaire ne veut rien dire dans l'abstrait — planter des arbustes, faire une robotomie et gracier quelqu'un sont trois actions complètement différentes. Seule la classe concrète sait quoi faire.

### `checkExecutable()` : factoriser les vérifications communes
```cpp
void AForm::checkExecutable(Bureaucrat const &executor) const
{
    if (!this->signedStatus)
        throw AForm::FormNotSignedException();
    if (executor.getGrade() > this->execGrade)
        throw AForm::GradeTooLowException();
}
```
Même si chaque formulaire fait une action différente, ils doivent **tous** vérifier la même chose avant de s'exécuter : "suis-je signé ?" et "le Bureaucrat qui m'exécute a-t-il le grade suffisant ?". Plutôt que de recopier ce code dans les 3 classes filles (ce qui serait source d'erreurs et de duplication), on l'écrit **une seule fois** dans la classe de base `AForm`, et chaque classe fille l'appelle en premier :
```cpp
void ShrubberyCreationForm::execute(Bureaucrat const &executor) const
{
    this->checkExecutable(executor);
    // ... action spécifique ...
}
```
C'est le principe de **factorisation** : le code commun vit à un seul endroit, le code spécifique vit dans chaque classe fille.

### Pourquoi chaque formulaire concret redéfinit un constructeur, un destructeur, `operator=` et un constructeur de copie ?
C'est la **forme canonique orthodoxe** (Orthodox Canonical Form) : toute classe non-exception du module doit fournir ces 4 éléments, même quand ils semblent "vides" ou triviaux. C'est une convention C++ qui garantit qu'un objet peut toujours être construit, copié, affecté et détruit proprement, sans surprise pour qui utilise la classe plus tard.

**Point important corrigé dans cette version** : quand une classe fille (par exemple `ShrubberyCreationForm`) redéfinit son `operator=`, elle doit penser à appeler **aussi** l'`operator=` de la classe de base, sinon la partie "héritée" de l'objet (ici `signedStatus`, qui vit dans `AForm`) ne serait jamais recopiée :
```cpp
ShrubberyCreationForm &ShrubberyCreationForm::operator=(ShrubberyCreationForm const &other)
{
    if (this != &other)
    {
        AForm::operator=(other);   // recopie la partie AForm (signedStatus)
        this->target = other.target; // recopie la partie propre à cette classe
    }
    return *this;
}
```
C'est un piège classique de l'héritage en C++ : chaque classe n'est responsable que de "sa" partie des données, donc il faut explicitement demander à la classe de base de s'occuper de la sienne.

### Pourquoi `Bureaucrat::executeForm()` ressemble tant à `signForm()` ?
```cpp
void Bureaucrat::executeForm(AForm const &form) const
{
    try
    {
        form.execute(*this);
        std::cout << this->name << " executed " << form.getName() << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cout << this->name << " couldn't execute " << form.getName()
                   << " because " << e.what() << std::endl;
    }
}
```
Même logique que pour signer : le Bureaucrat tente l'action, et si `form.execute()` lance une exception (formulaire non signé, grade insuffisant), on l'attrape ici pour afficher un message clair plutôt que de laisser planter le programme.

---

## Ex03 — La classe `Intern`

### L'idée
Un stagiaire (`Intern`) n'a ni nom ni grade : sa seule utilité est de savoir **fabriquer** un formulaire à partir d'un nom donné en texte ("robotomy request", par exemple), sans que l'appelant ait besoin de connaître à l'avance quelle classe C++ concrète correspond à ce nom.

### Pourquoi éviter les `if/else if/else` en cascade ?
Une solution "naïve" ressemblerait à :
```cpp
if (formName == "shrubbery creation")
    return new ShrubberyCreationForm(target);
else if (formName == "robotomy request")
    return new RobotomyRequestForm(target);
else if (formName == "presidential pardon")
    return new PresidentialPardonForm(target);
else
    ...
```
Ça marche, mais ça devient vite imbuvable si on ajoute 10 formulaires de plus, et c'est facile de se tromper ou d'oublier un cas. Le sujet demande donc d'utiliser un **tableau de pointeurs sur fonctions membres** : au lieu d'un enchaînement de tests, on construit une petite "table de correspondance" entre un nom et la fonction à appeler.

### Comment marche un pointeur sur fonction membre ?
```cpp
typedef AForm *(Intern::*FormCreator)(std::string const &) const;
```
Cette ligne définit un **type** : "un pointeur vers n'importe quelle méthode de `Intern` qui prend une `std::string const&` et renvoie un `AForm*`". C'est différent d'un pointeur de fonction classique, car une méthode a toujours besoin d'un objet (`this`) sur lequel s'exécuter — le pointeur "se souvient" seulement de quelle fonction appeler, pas sur quel objet.

```cpp
FormCreator const creators[3] = {
    &Intern::createShrubbery,
    &Intern::createRobotomy,
    &Intern::createPresidential
};
```
On stocke ici les **adresses** des trois méthodes privées de `Intern` (chacune sait créer un seul type de formulaire). Puis, pour appeler l'une d'elles dynamiquement :
```cpp
(this->*creators[i])(target);
```
La syntaxe `this->*creators[i]` veut dire "appelle la fonction pointée par `creators[i]`, sur l'objet `*this`". C'est un peu déroutant à lire la première fois, mais l'idée reste : "voici une case dans un tableau, elle contient une fonction à exécuter sur moi-même".

### `makeForm()` dans son ensemble
```cpp
AForm *Intern::makeForm(std::string const &formName, std::string const &target) const
{
    std::string const names[3] = { "shrubbery creation", "robotomy request", "presidential pardon" };
    FormCreator const creators[3] = { &Intern::createShrubbery, &Intern::createRobotomy, &Intern::createPresidential };

    for (int i = 0; i < 3; ++i)
    {
        if (names[i] == formName)
        {
            std::cout << "Intern creates " << formName << std::endl;
            return (this->*creators[i])(target);
        }
    }
    std::cout << "Intern couldn't find the form: " << formName << std::endl;
    return NULL;
}
```
On parcourt les noms connus avec une simple boucle `for` (pas de if/else en cascade sur le contenu métier), et dès qu'on trouve une correspondance, on appelle la bonne fonction via le tableau de pointeurs. Si aucun nom ne correspond, on renvoie `NULL` et on prévient l'utilisateur — c'est important de gérer ce cas, sinon le programme planterait en essayant d'utiliser un pointeur invalide plus tard.

### Pourquoi le `main` vérifie `if (robot)` avant d'utiliser le formulaire ?
```cpp
AForm *robot = intern.makeForm("robotomy request", "Bender");
if (robot)
{
    boss.signForm(*robot);
    boss.executeForm(*robot);
    delete robot;
}
```
Comme `makeForm()` peut renvoyer `NULL` (nom de formulaire inconnu), il faut vérifier le pointeur avant de l'utiliser, sinon on ferait un **déréférencement de pointeur nul**, ce qui plante le programme immédiatement (segfault).

### Pourquoi `delete robot;` à la fin ?
`makeForm()` fait un `new` (allocation sur le tas / heap). En C++, **toute mémoire allouée avec `new` doit être libérée avec `delete`**, sinon c'est une fuite mémoire (memory leak) : la mémoire reste occupée même après que le programme n'en a plus besoin. C'est la responsabilité de qui reçoit le pointeur (ici, le `main`) de le détruire une fois qu'il ne s'en sert plus.

---

## Résumé des idées transversales du module

| Notion | Où on la voit | Pourquoi elle existe |
|---|---|---|
| `const` sur un attribut | `name` dans `Bureaucrat`/`Form` | Empêcher qu'un attribut d'identité change après la création de l'objet |
| Exceptions personnalisées | `GradeTooHighException`, etc. | Signaler un état invalide sans planter le programme, en laissant l'appelant décider quoi faire |
| `try` / `catch` | `signForm()`, `executeForm()` | Transformer une exception technique en message clair pour l'utilisateur |
| Classe abstraite (`= 0`) | `AForm::execute()` | Forcer chaque classe fille à fournir sa propre version d'une action qui n'a pas de sens "en général" |
| Factorisation dans la classe de base | `AForm::checkExecutable()` | Éviter de dupliquer le même code de vérification dans chaque classe fille |
| Forme canonique orthodoxe | Constructeur/destructeur/copie/`operator=` partout | Garantir qu'un objet peut toujours être créé, copié et détruit proprement |
| Pointeurs sur fonctions membres | `Intern::makeForm()` | Remplacer une cascade de `if/else` par une table de correspondance nom → fonction, plus propre et plus facile à étendre |
| `new` / `delete` | `Intern::makeForm()` + `main` | Allouer un objet dont la durée de vie dépasse la fonction qui le crée, et éviter les fuites mémoire en le libérant explicitement |
