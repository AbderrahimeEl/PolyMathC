#include <stdio.h>
#include <stdlib.h>

typedef struct Monomial
{
    double coefficient;
    int degree;
} Monomial;

typedef struct Node
{
    Monomial term;
    struct Node *next;
    struct Node *previous;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
    int size;
} Polynomial;

Monomial multiplyMonomial(Monomial term1, Monomial term2);
Polynomial *createPolynomial();
void insertTerm(Polynomial *polynomial, Monomial term);
Polynomial *addPolynomial(const Polynomial *first_polynomial, const Polynomial *second_polynomial);
Polynomial *multiplyPolynomial(const Polynomial *first_polynomial, const Polynomial *second_polynomial);
Polynomial *derivePolynomial(Polynomial *p);
Polynomial *getPrimitivePolynomial(Polynomial *p);
void printPolynomial(const Polynomial *polynomial);
int isPolynomialEmpty(Polynomial *P);

int main()
{
    Polynomial *P1, *P2;
    P1 = createPolynomial();
    P2 = createPolynomial();
    Monomial term1, term2, term3, term4, term5, term6, term7, term8;
    term1.coefficient = 1;
    term1.degree = 1;
    term2.coefficient = 2;
    term2.degree = 2;
    term3.coefficient = 3;
    term3.degree = 3;
    term4.coefficient = 4;
    term4.degree = 4;
    term5.coefficient = 5;
    term5.degree = 5;
    term6.coefficient = 6;
    term6.degree = 6;
    term7.coefficient = 7;
    term7.degree = 7;
    term8.coefficient = 8;
    term8.degree = 8;
    insertTerm(P1, term1);
    insertTerm(P1, term4);
    insertTerm(P1, term5);
    insertTerm(P1, term7);
    printPolynomial(P1);
    printf("\nPrimitive :\n");
    Polynomial *primitive = getPrimitivePolynomial(P1);
    printPolynomial(primitive);
    return 0;
}

Polynomial *createPolynomial()
{
    Polynomial *P;
    P = (Polynomial *)malloc(sizeof(Polynomial));
    P->head = NULL;
    P->tail = NULL;
    P->size = 0;
    return P;
}

void insertTerm(Polynomial *polynomial, Monomial term)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->next = NULL;
    node->previous = NULL;
    node->term = term;
    if (polynomial->size == 0)
    {
        polynomial->head = node;
        polynomial->tail = node;
        polynomial->size++;
        return;
    }
    Node *temp = polynomial->head;
    // start insertion
    if (temp->term.degree >= node->term.degree)
    {
        if (temp->term.degree == node->term.degree)
        {
            if (temp->term.coefficient + node->term.coefficient == 0)
            {
                polynomial->head = polynomial->head->next;
                if (polynomial->size == 1)
                    polynomial->tail = polynomial->head;
                polynomial->head->previous = NULL;
                free(temp);
            }
            else
                temp->term.coefficient += node->term.coefficient;
            free(node);
            polynomial->size--;
            return;
        }
        polynomial->head->previous = node;
        node->next = polynomial->head;
        polynomial->head = node;
        polynomial->size++;
        return;
    }
    // end insertion
    temp = polynomial->tail;
    if (polynomial->tail->term.degree <= node->term.degree)
    {
        if (polynomial->tail->term.degree == node->term.degree)
        {
            if (polynomial->tail->term.coefficient == -node->term.coefficient)
            {
                polynomial->tail = polynomial->tail->previous;
                polynomial->tail->next = NULL;
                free(temp);
            }
            else
                polynomial->tail->term.coefficient += node->term.coefficient;
            free(node);
            polynomial->size--;
            return;
        }
        polynomial->tail->next = node;
        node->previous = polynomial->tail;
        polynomial->tail = node;
        polynomial->size++;
        return;
    }
    // middle insertion
    temp = polynomial->head;
    Node *temp2 = temp->next;
    while (temp->next != NULL)
    {
        if (temp->next->term.degree >= node->term.degree)
        {
            if (temp->next->term.degree == node->term.degree)
            {
                if (temp->next->term.coefficient == -node->term.coefficient)
                {
                    temp->next = temp2->next;
                    temp2->next->previous = temp;
                    free(temp2);
                }
                else
                    temp->next->term.coefficient += node->term.coefficient;
                free(node);
                polynomial->size--;
                return;
            }
            node->previous = temp;
            temp->next = node;
            node->next = temp2;
            temp2->previous = node;
            polynomial->size++;
            return;
        }
        temp = temp->next;
        if (temp != NULL)
            temp2 = temp->next;
    }
}

void printPolynomial(const Polynomial *polynomial)
{
    Node *temp;
    temp = polynomial->head;
    while (temp != NULL)
    {
        printf("%.2f * X%d   +   ", temp->term.coefficient, temp->term.degree);
        temp = temp->next;
    }
    printf("\tsize = %d", polynomial->size);
    printf("\n");
}

int isPolynomialEmpty(Polynomial *P)
{
    if (P->head == NULL)
        return 1;
    return 0;
}

Polynomial *addPolynomial(const Polynomial *first_polynomial, const Polynomial *second_polynomial)
{
    Polynomial *PolynomialSum;
    PolynomialSum = createPolynomial();
    Node *temp;
    temp = first_polynomial->head;
    while (temp != NULL)
    {
        insertTerm(PolynomialSum, temp->term);
        temp = temp->next;
    }
    temp = second_polynomial->head;
    while (temp != NULL)
    {
        insertTerm(PolynomialSum, temp->term);
        temp = temp->next;
    }
    return PolynomialSum;
}

Monomial multiplyMonomial(Monomial term1, Monomial term2)
{
    Monomial result;
    result.coefficient = term1.coefficient * term2.coefficient;
    result.degree = term1.degree + term2.degree;
    return result;
}

Polynomial *multiplyPolynomial(const Polynomial *first_polynomial, const Polynomial *second_polynomial)
{
    Polynomial *PolynomialResult = createPolynomial();
    Monomial term;
    Node *temp1, *temp2;
    temp1 = first_polynomial->head;
    temp2 = second_polynomial->head;
    while (temp1 != NULL)
    {
        temp2 = second_polynomial->head;
        while (temp2 != NULL)
        {
            term = multiplyMonomial(temp1->term, temp2->term);
            insertTerm(PolynomialResult, term);
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
    return PolynomialResult;
}

Polynomial *derivePolynomial(Polynomial *p)
{
    Polynomial *derivative;
    Node *temp = p->head;
    Monomial term;
    derivative = createPolynomial();
    while (temp != NULL)
    {
        if (temp->term.degree != 0)
        {
            term.coefficient = temp->term.coefficient * temp->term.degree;
            term.degree = temp->term.degree - 1;
            insertTerm(derivative, term);
        }
        temp = temp->next;
    }
    return derivative;
}

Polynomial *getPrimitivePolynomial(Polynomial *p)
{
    Polynomial *primitive;
    Node *temp = p->head;
    Monomial term;
    primitive = createPolynomial();
    while (temp != NULL)
    {
        if (temp->term.degree != 0)
        {
            term.coefficient = temp->term.coefficient / (temp->term.degree + 1);
            term.degree = temp->term.degree + 1;
            insertTerm(primitive, term);
        }
        temp = temp->next;
    }
    return primitive;
}
