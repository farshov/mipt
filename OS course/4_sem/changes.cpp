#include "changes.hpp"


Node::Node():
value(new char[MAX_STR_SIZE]),
left(NULL),
right(NULL),
typeDerivative(true)
{
    value[0] = '\0';
}

Node::~Node()
{
    delete[] value;
    delete left;
    delete right;
}

void Node::PrintTree()
{
    printf("%s", value);

    if (left != NULL)
        left -> PrintTree();
    if (right != NULL)
        right -> PrintTree();
}

void Node::PrintToFile(FILE *fp)
{
    fprintf(fp, "( ");

    if (left)
        left -> PrintToFile(fp);
    else
        fprintf(fp, "%s", "null ");

    fprintf(fp, "%s ", value);

    if (right)
        right -> PrintToFile(fp);
    else
        fprintf(fp, "%s", "nulr ");

    fprintf(fp, ") ");
}

void Node::BuildGraphWithGraphViz(FILE* fp)
{
    fprintf(fp, "TreeNode_%p [label=\"%s\\l\"]\n", this, this -> value);
    if (this -> left != NULL)
    {
        this -> left -> BuildGraphWithGraphViz(fp);
        fprintf(fp, "TreeNode_%p -> TreeNode_%p\n", this, this -> left);
        fprintf(fp, "TreeNode_%p [label=\"%s\\l\"]\n", this, this -> value);
    }
    if (this -> right != NULL)
    {
        this -> right -> BuildGraphWithGraphViz(fp);
        fprintf(fp, "TreeNode_%p -> TreeNode_%p\n", this, this -> right);
        fprintf(fp, "TreeNode_%p [label=\"%s\\l\"]\n", this, this -> value);
    }
}

void Node::GoDownForRefuseDer()
{
    this -> typeDerivative = false;

    if (left != NULL)
        left -> GoDownForRefuseDer();
    if (right != NULL)
        right -> GoDownForRefuseDer();
}

void Node::ReadingFromFile(FILE *fp)
{
    char tmp[MAX_STR_SIZE] = "";
    fscanf(fp, "%s ", tmp);

    if (tmp[0] != '(')
    {
        fputs("Ошибка чтения\n", stderr);
        return;
    }

    fscanf(fp, "%s ", tmp);

    if (strncmp(tmp, "null", 4) == 0)
        this -> left = NULL;
    else
        if (strncmp(tmp, "(", 1) == 0)
        {
            Node *left_ = new Node();
            this -> left = left_;
            fseek(fp, -2, SEEK_CUR);
            left_ -> ReadingFromFile(fp);
        }

    fscanf(fp, "%s ", tmp);
    bool switcher = false;
    if ((strcmp(tmp, deg_str) == 0) || (strcmp(tmp, mul_str) == 0) || (strcmp(tmp, add_str) == 0) || (strcmp(tmp, dev_str) == 0))
    {
        strncpy(value, tmp, MAX_STR_SIZE);
        this -> type = OPERAND;
    }
    else
    {
        if (strcmp(tmp, null_str) != 0)
        {
            int value_str = atoi(tmp);
            if (value_str)
            {
                strncpy(value, tmp, MAX_STR_SIZE);
                this -> type = VALUE;
            }
            else
            {
                strncpy(value, tmp, MAX_STR_SIZE);
                this -> type = VARIABLE;
                if ((strcmp(tmp, sin_str) == 0) || (strcmp(tmp, cos_str) == 0) || (strcmp(tmp, ln_str) == 0) || (strcmp(tmp, log_str) == 0))
                    switcher = true;
            }
        }
        else
        {
            strncpy(value, tmp, MAX_STR_SIZE);
            this -> type = VALUE;
        }
    }

    fscanf(fp, "%s ", tmp);
    if (strncmp(tmp, "(", 1) == 0)
    {
        if (!switcher)
        {
            Node *right_ = new Node();
            this -> right = right_;
            fseek(fp, -2, SEEK_CUR);
            right_ -> ReadingFromFile(fp);
        }
        else
        {
            char new_tmp[MAX_STR_SIZE + 1] = "";
            fscanf(fp, "%s ", new_tmp);
            if (strcmp(new_tmp, "(") == 0)
            {
                printf("Ввели неверую формулу\n");
                exit (100);
            }
            this -> right = NULL;
            fseek(fp, 8, SEEK_CUR);
        }
    }
    else if (strncmp(tmp, "nulr", 4) == 0)
        this -> right = NULL;

    fscanf(fp, "%s ", tmp);
}

void Node::ChangesForOther()
{
    if (strcmp(this -> value, sh_str) == 0)
    {
        Node* NewNodeVariableRight = new Node();
        AssignmentFunc(NewNodeVariableRight, VARIABLE, false);
        strcpy(NewNodeVariableRight -> value, ch_str);

        Node* NewNodeRightLeft = new Node();
        CopyNodeTotal(NewNodeRightLeft, this -> left);
        NewNodeRightLeft -> GoDownForRefuseDer();

        strcpy(this -> value, mul_str);
        ConnectionNode(this -> left, this, NewNodeVariableRight);
        ConnectionNode(NewNodeRightLeft, NewNodeVariableRight, NULL);
    }
    else if (strcmp(this -> value, ch_str) == 0)
    {
        Node* NewNodeVariableRight = new Node();
        AssignmentFunc(NewNodeVariableRight, VARIABLE, false);
        strcpy(NewNodeVariableRight -> value, sh_str);

        Node* NewNodeRightLeft = new Node();
        CopyNodeTotal(NewNodeRightLeft, this -> left);
        NewNodeRightLeft -> GoDownForRefuseDer();

        strcpy(this -> value, mul_str);
        ConnectionNode(this -> left, this, NewNodeVariableRight);
        ConnectionNode(NewNodeRightLeft, NewNodeVariableRight, NULL);
    }
    else if (strcmp(this -> value, tg_str) == 0)
    {
        Node* NewNodeValueRightLeft = new Node();
        AssignmentFunc(NewNodeValueRightLeft, VALUE, false);
        strcpy(NewNodeValueRightLeft -> value, unit_str);

        Node* NewNodeRightRightLeftLeft = new Node();
        CopyNodeTotal(NewNodeRightRightLeftLeft, this -> left);
        NewNodeRightRightLeftLeft -> GoDownForRefuseDer();

        Node* NewNodeVariableRightRightLeft = new Node();
        AssignmentFunc(NewNodeVariableRightRightLeft, VARIABLE, false);
        strcpy(NewNodeVariableRightRightLeft -> value, cos_str);

        Node* NewNodeValueRightRightRight = new Node();
        AssignmentFunc(NewNodeValueRightRightRight, VALUE, false);
        strcpy(NewNodeValueRightRightRight -> value, deuce_str);

        Node* NewNodeOperandRight = new Node();
        AssignmentFunc(NewNodeOperandRight, OPERAND, false);
        strcpy(NewNodeOperandRight -> value, dev_str);

        Node* NewNodeOperandRightRight = new Node();
        AssignmentFunc(NewNodeOperandRightRight, OPERAND, false);
        strcpy(NewNodeOperandRightRight -> value, deg_str);

        strcpy(this -> value, mul_str);
        AssignmentFunc(this, OPERAND, false);

        ConnectionNode(this -> left, this, NewNodeOperandRight);
        ConnectionNode(NewNodeValueRightLeft, NewNodeOperandRight, NewNodeOperandRightRight);
        ConnectionNode(NewNodeVariableRightRightLeft, NewNodeOperandRightRight, NewNodeValueRightRightRight);
        ConnectionNode(NewNodeRightRightLeftLeft, NewNodeVariableRightRightLeft, NULL);
    }
    else if (strcmp(this -> value, ctg_str) == 0)
    {
        Node* NewNodeValueRightLeft = new Node();
        AssignmentFunc(NewNodeValueRightLeft, VALUE, false);
        strcpy(NewNodeValueRightLeft -> value, neg_unit_str);

        Node* NewNodeRightRightRightLeftLeft = new Node();
        CopyNodeTotal(NewNodeRightRightRightLeftLeft, this -> left);
        NewNodeRightRightRightLeftLeft -> GoDownForRefuseDer();

        Node* NewNodeVariableRightRightRightLeft = new Node();
        AssignmentFunc(NewNodeVariableRightRightRightLeft, VARIABLE, false);
        strcpy(NewNodeVariableRightRightRightLeft -> value, sin_str);

        Node* NewNodeValueRightRightLeft = new Node();
        AssignmentFunc(NewNodeValueRightRightLeft, VALUE, false);
        strcpy(NewNodeValueRightRightLeft -> value, unit_str);

        Node* NewNodeValueRightRightRightRight = new Node();
        AssignmentFunc(NewNodeValueRightRightRightRight, VALUE, false);
        strcpy(NewNodeValueRightRightRightRight -> value, deuce_str);

        Node* NewNodeOperandRight = new Node();
        AssignmentFunc(NewNodeOperandRight, OPERAND, false);
        strcpy(NewNodeOperandRight -> value, mul_str);

        Node* NewNodeOperandRightRight = new Node();
        AssignmentFunc(NewNodeOperandRightRight, OPERAND, false);
        strcpy(NewNodeOperandRightRight -> value, dev_str);

        Node* NewNodeOperandRightRightRight = new Node();
        AssignmentFunc(NewNodeOperandRightRightRight, OPERAND, false);
        strcpy(NewNodeOperandRightRightRight -> value, deg_str);

        strcpy(this -> value, mul_str);
        AssignmentFunc(this, OPERAND, false);

        this -> right = NewNodeOperandRight;
        ConnectionNode(NewNodeValueRightLeft, NewNodeOperandRight, NewNodeOperandRightRight);
        ConnectionNode(NewNodeValueRightRightLeft, NewNodeOperandRightRight, NewNodeOperandRightRightRight);
        ConnectionNode(NewNodeVariableRightRightRightLeft, NewNodeOperandRightRightRight, NewNodeValueRightRightRightRight);
        ConnectionNode(NewNodeRightRightRightLeftLeft, NewNodeVariableRightRightRightLeft, NULL);
    }
    else if (strcmp(this -> value, sin_str) == 0)
    {
        strcpy(this -> value, mul_str);
        AssignmentFunc(this, OPERAND, false);

        Node* NewNodeLeftLeft = new Node();
        CopyNodeTotal(NewNodeLeftLeft, this -> left);
        NewNodeLeftLeft -> GoDownForRefuseDer();

        Node* NewNodeRight = new Node();
        CopyNodeTotal(NewNodeRight, this -> left);

        Node* NewVariableLeft = new Node();
        AssignmentFunc(NewVariableLeft, VARIABLE, false);
        strcpy(NewVariableLeft -> value, cos_str);

        ConnectionNode(NewNodeLeftLeft, NewVariableLeft, NULL);
        ConnectionNode(NewVariableLeft, this, NewNodeRight);
    }
    else if (strcmp(this -> value, cos_str) == 0)
    {
        strcpy(this -> value, mul_str);
        AssignmentFunc(this, OPERAND, false);

        Node* NewNodeLeftLeftLeft = new Node ();
        CopyNodeTotal(NewNodeLeftLeftLeft, this -> left);
        NewNodeLeftLeftLeft -> GoDownForRefuseDer();

        Node* NewOperandLeft = new Node ();
        AssignmentFunc(NewOperandLeft, OPERAND, false);
        strcpy(NewOperandLeft -> value, mul_str);

        Node* NewVariableLeftLeft = new Node ();
        AssignmentFunc(NewVariableLeftLeft, VARIABLE, false);
        strcpy(NewVariableLeftLeft -> value, sin_str);

        Node* NewNodeLeftRight = new Node ();
        AssignmentFunc(NewNodeLeftRight, VALUE, false);
        strcpy(NewNodeLeftRight -> value, neg_unit_str);

        Node* NewNodeRight = new Node ();
        CopyNodeTotal(NewNodeRight, this -> left);

        ConnectionNode(NewOperandLeft, this, NewNodeRight);
        ConnectionNode(NewVariableLeftLeft, NewOperandLeft, NewNodeLeftRight);
        ConnectionNode(NewNodeLeftLeftLeft, NewVariableLeftLeft, NULL);
    }
    else if (strcmp(this -> value, ln_str) == 0)
    {
        strcpy(this -> value, mul_str);
        AssignmentFunc(this, OPERAND, false);

        Node* NewNodeLeftLeft = new Node();
        AssignmentFunc(NewNodeLeftLeft, VALUE, false);
        strcpy(NewNodeLeftLeft -> value, unit_str);

        Node* NewNodeLeftRight = new Node();
        CopyNodeTotal(NewNodeLeftRight, this -> left);
        NewNodeLeftRight -> GoDownForRefuseDer();

        Node* NewNodeRight = new Node();
        CopyNodeTotal(NewNodeRight, this -> left);

        Node* NewOperandLeft = new Node();
        AssignmentFunc(NewNodeLeftLeft, OPERAND, false);
        strcpy(NewOperandLeft -> value, dev_str);

        ConnectionNode(NewNodeRight, this, NewOperandLeft);
        ConnectionNode(NewNodeLeftLeft, NewOperandLeft, NewNodeLeftRight);
}

    else if (strcmp(this -> value, var_str) == 0)
    {
        strcpy(this -> value, unit_str);
        this -> type = VALUE;
        this -> typeDerivative = false;
        ConnectionNode(NULL, this, NULL);
    }
}

void Node::ChangesForDeg()
{
    if (this -> right -> type == VALUE)
    {
        int value_deg = 0;
        if (strcmp(this -> right -> value, null_str) != 0)
        {
            value_deg = atoi(this -> right -> value);
            if (value_deg == 0)
            {
                fputs("Ошибка atoi\n", stderr);
                exit(0);
            }
        }

        char* str_value_deg = new char(MAX_STR_SIZE);
        char* str_value_mul = new char(MAX_STR_SIZE);
        str_value_deg[0] = '\0';
        str_value_mul[0] = '\0';
        sprintf(str_value_deg, "%d", value_deg - 1);
        sprintf(str_value_mul, "%d", value_deg);

        strcpy(this -> value, mul_str);
        AssignmentFunc(this, OPERAND, false);

        Node* NewOperandLeft = new Node ();
        strcpy(NewOperandLeft -> value, mul_str);
        AssignmentFunc(NewOperandLeft, OPERAND, false);

        Node* NewOperandLeftLeft = new Node ();
        strcpy(NewOperandLeftLeft -> value, deg_str);
        AssignmentFunc(NewOperandLeftLeft, OPERAND, false);

        Node* NewNodeLeftLeftLeft = new Node();
        CopyNodeTotal(NewNodeLeftLeftLeft, this -> left);
        NewNodeLeftLeftLeft -> GoDownForRefuseDer();

        Node* NewNodeRight = new Node();
        CopyNodeTotal(NewNodeRight, this -> left);

        Node* NewNodeLeftRight = new Node();
        strcpy(NewNodeLeftRight -> value, str_value_mul);
        AssignmentFunc(NewNodeLeftRight, VALUE, false);

        Node* NewNodeLeftLeftRight = new Node();
        AssignmentFunc(NewNodeLeftLeftRight, VALUE, false);
        strcpy(NewNodeLeftLeftRight -> value, str_value_deg);

        strcpy(this -> left -> value, deg_str);
        AssignmentFunc(this -> left, OPERAND, false);

        ConnectionNode(NewOperandLeftLeft, NewOperandLeft, NewNodeLeftRight);
        ConnectionNode(NewNodeLeftLeftLeft, NewOperandLeftLeft, NewNodeLeftLeftRight);
        ConnectionNode(NewOperandLeft, this, NewNodeRight);
        delete str_value_deg;
        delete str_value_mul;
    }
    else if (this -> left -> type == VALUE)
    {
        strcpy(this -> value, mul_str);
        AssignmentFunc(this, OPERAND, false);

        Node* NewNodeLeftLeftLeft = new Node ();
        CopyNodeSpecial(NewNodeLeftLeftLeft, this -> left);
        NewNodeLeftLeftLeft -> typeDerivative = false;

        Node* NewNodeLeftLeftRight = new Node ();
        CopyNodeTotal(NewNodeLeftLeftRight, this -> right);
        NewNodeLeftLeftRight -> GoDownForRefuseDer();

        Node* NewNodeLeftRightLeft = new Node();
        AssignmentFunc(NewNodeLeftRightLeft, VALUE, false);
        char str_for_value[MAX_STR_SIZE + 10] = "ln ";//может быть перебор чисел
        strcat(str_for_value, this -> left -> value);
        strcpy(NewNodeLeftRightLeft -> value, str_for_value);

        Node* NewOperandLeft = new Node();
        AssignmentFunc(NewOperandLeft, OPERAND, false);
        strcpy(NewOperandLeft -> value, mul_str);

        Node* NewOperandLeftLeft = new Node();
        AssignmentFunc(NewOperandLeftLeft, OPERAND, false);
        strcpy(NewOperandLeftLeft -> value, deg_str);

        Node *NewValueLeft = new Node();
        AssignmentFunc(NewValueLeft, VALUE, false);
        strcpy(NewOperandLeft -> value, mul_str);

        ConnectionNode(NewOperandLeft, this, this -> right);
        ConnectionNode(NewOperandLeftLeft, NewOperandLeft, NewNodeLeftRightLeft);
        ConnectionNode(NewNodeLeftLeftLeft, NewOperandLeftLeft, NewNodeLeftLeftRight);
    }
}

void Node::ChangesForDev()
{
    if (this -> right -> type != VALUE)
    {
        Node* NewNodeLeftLeftLeft = new Node();
        CopyNodeTotal(NewNodeLeftLeftLeft, this -> left);

        Node* NewNodeLeftLeftRight = new Node();
        CopyNodeTotal(NewNodeLeftLeftRight, this -> right);
        NewNodeLeftLeftRight -> GoDownForRefuseDer();

        Node* NewNodeLeftRightLeft = new Node();
        CopyNodeTotal(NewNodeLeftRightLeft, this -> left);
        NewNodeLeftRightLeft -> GoDownForRefuseDer();

        Node* NewNodeLeftRightRight = new Node();
        CopyNodeTotal(NewNodeLeftRightRight, this -> right);

        Node* NewNodeRightLeft = new Node();
        CopyNodeTotal(NewNodeRightLeft, this -> right);
        NewNodeRightLeft -> GoDownForRefuseDer();

        Node* NewNodeRightRight = new Node();
        AssignmentFunc(NewNodeRightRight, VALUE, false);
        strcpy(NewNodeRightRight -> value, deuce_str);

        strcpy(this -> left -> value, sub_str);
        AssignmentFunc(this -> left, OPERAND, false);

        strcpy(this -> right -> value, deg_str);
        AssignmentFunc(this -> right, OPERAND, false);

        Node* NewOperandLeftLeft = new Node();
        strcpy(NewOperandLeftLeft -> value, mul_str);
        AssignmentFunc(NewOperandLeftLeft, OPERAND, false);

        Node* NewOperandLeftRight = new Node();
        strcpy(NewOperandLeftRight -> value, mul_str);
        AssignmentFunc(NewOperandLeftRight, OPERAND, false);

        ConnectionNode(NewOperandLeftLeft, this -> left, NewOperandLeftRight);
        ConnectionNode(NewNodeLeftLeftLeft, NewOperandLeftLeft, NewNodeLeftLeftRight);
        ConnectionNode(NewNodeLeftRightLeft, NewOperandLeftRight, NewNodeLeftRightRight);
        ConnectionNode(NewNodeRightLeft, this -> right, NewNodeRightRight);
    }
    else if (this -> right -> type == VALUE)
    {
        strcpy(this -> value, mul_str);
        this -> typeDerivative = false;
        int len_value = (int)strlen(this -> right -> value);
        char *value_str = new char[3 + len_value];
        strcpy(value_str, "1/");
        strcat(value_str, this -> right -> value);
        strcpy (this -> right -> value, value_str);
        this -> right -> typeDerivative = false;
    }
}

void Node::ChangesForMul()
{
    Node* NewNodeLeftLeft = new Node();
    Node* NewNodeLeftRight = new Node();
    Node* NewNodeRightLeft = new Node();
    Node* NewNodeRightRight = new Node();
    Node* NewNodeOperandLeft = new Node();
    Node* NewNodeOperandRight = new Node();
    CopyNodeTotal(NewNodeLeftRight, this -> right);
    NewNodeLeftRight -> GoDownForRefuseDer();

    CopyNodeTotal(NewNodeRightLeft, this -> left);
    NewNodeRightLeft -> GoDownForRefuseDer();

    CopyNodeTotal(NewNodeLeftLeft, this -> left);
    CopyNodeTotal(NewNodeRightRight, this -> right);

    strcpy(this -> value, add_str);
    this -> type = OPERAND;

    AssignmentFunc(NewNodeOperandLeft, OPERAND, false);
    strcpy(NewNodeOperandLeft -> value, mul_str);

    AssignmentFunc(NewNodeOperandRight, OPERAND, false);
    strcpy(NewNodeOperandRight -> value, mul_str);
    ConnectionNode(NewNodeOperandLeft, this, NewNodeOperandRight);
    ConnectionNode(NewNodeLeftLeft, this -> left, NewNodeLeftRight);
    ConnectionNode(NewNodeRightLeft, this -> right, NewNodeRightRight);
}

void Node::ProcDerivative()
{
    if (this -> type == OPERAND && this -> typeDerivative == true)
    {
        if (strcmp(this -> value, mul_str) == 0)
            this -> ChangesForMul();
        else if (strcmp(this -> value, dev_str) == 0)
            this -> ChangesForDev();
        else if (strcmp(this -> value, deg_str) == 0)
            this -> ChangesForDeg();
    }
    else if (this -> type == VARIABLE && this -> typeDerivative == true)
        this -> ChangesForOther();

    else if (this -> type == VALUE && this -> typeDerivative == true)
    {
        strcpy(this -> value, null_str);
        this -> typeDerivative = false;
    }
    if (this -> left)
        this -> left -> ProcDerivative();
    if (this -> right)
        this -> right -> ProcDerivative();
}

void AssignmentFunc(Node* node, int type, bool typeDiv) // присваивание (куда значение арг и тип дифф)
{
    if (type == VARIABLE)
        node -> type = VARIABLE;
    else if (type == OPERAND)
        node -> type = OPERAND;
    else if (type == VALUE)
        node -> type = VALUE;
    else
    {
        printf("AssignmentFunc сработала неверно");
        exit (100);
    }
    node -> typeDerivative = typeDiv;
}

void CopyNode(Node* b, Node* a)
{
    b -> type = a -> type;
    strcpy(b -> value, a -> value);
    b -> left = a -> left;
    b -> right = a -> right;
    b -> typeDerivative = a -> typeDerivative;
}

void CopyNodeSpecial(Node* b, Node* a)
{
    b -> type = a -> type;
    strcpy(b -> value, a -> value);
    b -> typeDerivative = a -> typeDerivative;
}

void CopyNodeTotal(Node* b, Node* a)
{
    CopyNodeSpecial(b, a);
    if (a -> left)
    {
        Node* left_ = new Node();
        CopyNodeSpecial(left_, a -> left);
        b -> left = left_;
        CopyNodeTotal(b -> left, a -> left);
    }
    if (a -> right)
    {
        Node* right_ = new Node();
        CopyNodeSpecial(right_, a -> right);
        b -> right = right_;
        CopyNodeTotal(b -> right, a -> right);
    }
}

void Node::Conventer()//дописать
{
    if ((this -> left) && (this -> right) && (this -> left -> type != VALUE) && (strcmp(this -> value, deg_str) == 0) && (this -> right -> type != VALUE))
    {
        Node* NewNodeRightRightRight = new Node();
        CopyNodeTotal(NewNodeRightRightRight, this -> right);

        Node* NewNodeRightLeft = new Node();
        CopyNodeTotal(NewNodeRightLeft, this -> left);

        Node* NewVariableRightRight = new Node();
        AssignmentFunc(NewVariableRightRight, VARIABLE, true);
        strcpy(NewVariableRightRight -> value, ln_str);

        Node* NewOperandRight = new Node();
        AssignmentFunc(NewOperandRight, OPERAND, true);
        strcpy(NewOperandRight -> value, mul_str);

        AssignmentFunc(this -> left, VALUE, false);
        strcpy(this -> left -> value, exp_str);

        ConnectionNode(NULL, this -> left, NULL);
        ConnectionNode(this -> left, this, NewOperandRight);
        ConnectionNode(NewNodeRightLeft, this -> right, NewVariableRightRight);
        NewVariableRightRight -> left = NewNodeRightRightRight;
    }
    if (this -> left)
        this -> left -> Conventer();
    if (this -> right)
        this -> right -> Conventer();
}

void Node::BackConventer()// дописать
{
    if (strcmp(this -> value, deg_str) == 0 && strcmp(this -> left -> value, exp_str) == 0 && strcmp(this -> right -> value, mul_str) == 0 && strcmp(this -> right -> right -> value, ln_str) == 0)
    {
        Node* NewNodeLeft = new Node();
        Node* NewNodeRight = new Node();
        CopyNodeTotal(NewNodeLeft, this -> right -> right -> left);
        CopyNodeTotal(NewNodeRight, this -> right -> left);
        ConnectionNode(NewNodeLeft, this, NewNodeRight);
    }
    if (this -> left != NULL)
        this -> left -> BackConventer();
    if (this -> right != NULL)
        this -> right -> BackConventer();
}

void Node::BuildTexWithLaTex(FILE* TexFile)
{
    fprintf(TexFile, "{");
    if (strcmp(this -> value, add_str) == 0)
    {
        this -> left -> BuildTexWithLaTex(TexFile);
        fprintf(TexFile, add_str);
        this -> right -> BuildTexWithLaTex(TexFile);
    }
    else if (strcmp(this -> value, sub_str) == 0)
    {
        this -> left -> BuildTexWithLaTex(TexFile);
        fprintf(TexFile, sub_str);
        this -> right -> BuildTexWithLaTex(TexFile);
    }
    else if (strcmp(this -> value, mul_str) == 0)
    {
        if ((isdigitmore(this -> left -> value) && (strcmp(this -> right -> value, var_str) == 0)) || (isdigitmore(this -> right -> value) && (strcmp(this -> left -> value, var_str) == 0)) || (isdigitmore(this -> left -> value) && isdigitmore(this -> right -> value)) || ((strcmp(this -> left -> value, var_str) == 0) && (strcmp(this -> right -> value, var_str) == 0)))
        {
            this -> left -> BuildTexWithLaTex(TexFile);
            fprintf(TexFile, mul_str);
            this -> right -> BuildTexWithLaTex(TexFile);
        }
        else
        {
            fprintf(TexFile, "\\left(");
            this -> left -> BuildTexWithLaTex(TexFile);
            fprintf(TexFile, "\\right)");
            fprintf(TexFile, mul_str);
            fprintf(TexFile, "\\left(");
            this -> right -> BuildTexWithLaTex(TexFile);
            fprintf(TexFile, "\\right)");
        }
    }
    else if (strcmp(this -> value, dev_str) == 0)
    {
        fprintf(TexFile, "\\frac{");
        this -> left -> BuildTexWithLaTex(TexFile);
        fprintf(TexFile, "}{");
        this -> right -> BuildTexWithLaTex(TexFile);
        fprintf(TexFile, "}");
    }
    else if (strcmp(this -> value, deg_str) == 0)
    {
        if (this -> left -> left == NULL && this -> left -> right == NULL)
            this -> left -> BuildTexWithLaTex(TexFile);
        else
        {
            fprintf(TexFile, "(");
            this -> left -> BuildTexWithLaTex(TexFile);
            fprintf(TexFile, ")");
        }
        fprintf(TexFile, deg_str);
        fprintf(TexFile, "{");
        this -> right -> BuildTexWithLaTex(TexFile);
        fprintf(TexFile, "}");
    }
    else if ((strcmp(this -> value, sin_str) == 0) || (strcmp(this -> value, cos_str) == 0) || (strcmp(this -> value, ln_str) == 0) || (strcmp(this -> value, tg_str) == 0) || (strcmp(this -> value, ctg_str) == 0) || (strcmp(this -> value, sh_str) == 0) || (strcmp(this -> value, ch_str) == 0))
    {
        if (strcmp(this -> value, sin_str) == 0)
            fprintf(TexFile, "sin");
        else if (strcmp(this -> value, cos_str) == 0)
            fprintf(TexFile, "cos");
        else if (strcmp(this -> value, ln_str) == 0)
            fprintf(TexFile, "ln");
        else if (strcmp(this -> value, tg_str) == 0)
            fprintf(TexFile, "tg");
        else if (strcmp(this -> value, ctg_str) == 0)
            fprintf(TexFile, "ctg");
        else if (strcmp(this -> value, sh_str) == 0)
            fprintf(TexFile, "sh");
        else if (strcmp(this -> value, ch_str) == 0)
            fprintf(TexFile, "ch");
        
        fprintf(TexFile, "\\left(");
        this -> left -> BuildTexWithLaTex(TexFile);
        fprintf(TexFile, "\\right)");
    }
        
    else if (strcmp(this -> value, var_str) == 0)
        fprintf(TexFile, var_str);
    else
    {
        if (strcmp(this -> value, neg_unit_str) == 0)
        {
            fprintf(TexFile, "\\left(");
            fprintf(TexFile, "%s", this -> value);
            fprintf(TexFile, "\\right)");
        }
        else
            fprintf(TexFile, "%s", this -> value);
    }
    fprintf(TexFile, "}");
}

void Node::ProcessingTree()
{
    if (strcmp(this -> value , mul_str) == 0 && this -> left != NULL)
    {
        if ((strcmp(this -> left -> value, null_str) == 0) || (strcmp(this -> right -> value, null_str) == 0))
        {
            strcpy(this -> value, null_str);
            this -> type = VALUE;
            ConnectionNode(NULL, this, NULL);
        }
        else if (strcmp(this -> left -> value, unit_str) == 0 || strcmp(this -> left -> value, "ln e") == 0)
        {
            Node* NewNodeRight = new Node();
            CopyNodeTotal(NewNodeRight , this -> right);
            CopyNode(this, NewNodeRight);
        }
        else if (strcmp(this -> right -> value, unit_str) == 0 || strcmp(this -> right -> value, "ln e") == 0)
        {
            Node* NewNodeLeft = new Node();
            CopyNodeTotal(NewNodeLeft , this -> left);
            CopyNode(this, NewNodeLeft);
        }
        else if (strcmp(this -> right -> value, dev_str) == 0)
        {
            Node* NewNodeLeftLeft = new Node();
            CopyNodeTotal(NewNodeLeftLeft, this -> right -> left);
            
            Node* NewNodeLeftRight = new Node();
            CopyNodeTotal(NewNodeLeftRight, this -> left);
            
            Node* NewNodeRight = new Node();
            CopyNodeTotal(NewNodeRight, this -> right -> right);
            
            Node* NewNodeOperandLeft = new Node();
            CopyNodeSpecial(NewNodeOperandLeft, this);
            
            CopyNodeSpecial(this, this -> right);
            
            ConnectionNode(NewNodeOperandLeft, this, NewNodeRight);
            ConnectionNode(NewNodeLeftLeft, NewNodeOperandLeft, NewNodeLeftRight);
        }
        else if (strcmp(this -> left -> value, mul_str) == 0)//дописать
        {
            if(this -> right -> type == VALUE && this -> left -> right -> type == VALUE)
            {
                Node* NewNodeLeft = new Node();
                CopyNodeTotal(NewNodeLeft, this -> left -> left);
                
                Node* NewNodeValueRight = new Node();
                sprintf(NewNodeValueRight -> value, "%d",GetN1(this -> right -> value)*GetN1(this -> left -> right -> value));
                
                AssignmentFunc(NewNodeValueRight, VALUE, this -> typeDerivative);
                ConnectionNode(NewNodeLeft, this, NewNodeValueRight);
            }
        }
    }
    else if (strcmp(this -> value , dev_str) == 0)
    {
        if (strcmp(this -> left -> value, null_str) == 0)
        {
            strcpy(this -> value, null_str);
            AssignmentFunc(this, VALUE, this -> typeDerivative);
            ConnectionNode(NULL, this, NULL);
        }
        else if ((strcmp(this -> right -> value, unit_str) == 0) || (strcmp(this -> right -> value, "ln e") == 0))
        {
            Node* NewNodeLeft = new Node();
            CopyNode(NewNodeLeft , this -> left);
            CopyNode(this, NewNodeLeft);
        }
        else if(this -> left != NULL && this -> right != NULL)
            if (ComparisonNode(this -> left, this -> right))
        {
            strcpy(this -> value, unit_str);
            this -> type = VALUE;
            ConnectionNode(NULL, this, NULL);
        }
    }
    else if (strcmp(this -> value , deg_str) == 0)
    {
        if (strcmp(this -> right -> value, null_str) == 0)
        {
            strcpy(this -> value, unit_str);
            this -> type = VALUE;
            ConnectionNode(NULL, this, NULL);
        }
        else if (strcmp(this -> right -> value, unit_str) == 0)
        {
            Node* NewNodeLeft = new Node();
            CopyNodeTotal(NewNodeLeft , this -> left);
            CopyNode(this, NewNodeLeft);
        }
    }
    else if (strcmp(this -> value , add_str) == 0)
    {
        if (strcmp(this -> left -> value, null_str) == 0)
        {
            Node* NewNodeRight = new Node();
            CopyNodeTotal(NewNodeRight, this -> right);
            CopyNode(this, NewNodeRight);
        }
        else if (strcmp(this -> right -> value, null_str) == 0)
        {
            Node* NewNodeLeft = new Node();
            CopyNodeTotal(NewNodeLeft , this -> left);
            CopyNode(this, NewNodeLeft);
        }
        else if (ComparisonNode(this -> left, this -> right))
        {
            Node* NewNodeLeft = new Node();
            CopyNodeTotal(NewNodeLeft, this -> left);
            
            Node* NewNodeValueRight = new Node();
            strcpy(NewNodeValueRight -> value, deuce_str);
            AssignmentFunc(NewNodeValueRight, VALUE, this -> typeDerivative);
            
            strcpy(this -> value, mul_str);
            ConnectionNode(NewNodeLeft, this, NewNodeValueRight);
        }
    }
    else if (strcmp(this -> value , sub_str) == 0)
    {
        if (strcmp(this -> left -> value, null_str) == 0 && isdigitmore(this -> right -> value))
        {
            SingnedValue(this -> right -> value);
            CopyNode(this, this -> right);
            this -> type = VALUE;
            ConnectionNode(NULL, this, NULL);
        }
        else if (strcmp(this -> left -> value, null_str) == 0 && strcmp(this -> right -> value, null_str) == 0)
        {
            ConnectionNode(NULL, this, NULL);
            this -> type = VALUE;
            strcpy(this -> value, null_str);
        }
        else if (ComparisonNode(this -> left, this -> right))
        {
            strcpy(this -> value, null_str);
            AssignmentFunc(this, VALUE, this -> typeDerivative);
            ConnectionNode(NULL, this, NULL);
        }
    }
    if (this -> left != NULL)
        this -> left -> ProcessingTree();
    if (this -> right != NULL)
        this -> right -> ProcessingTree();
}

void openGraph(Node* tree)
{
    FILE *dotFile = fopen("my_graph.gv", "w");
    if (!dotFile)
    {
        fputs("Ошибка файла\n", stderr);
        exit(1);
    }

    fprintf(dotFile, "digraph G{\ngraph [dpi = 300]");
    tree -> BuildGraphWithGraphViz(dotFile);
    fprintf(dotFile, "}");
    fclose(dotFile);
}

bool ComparisonNode(Node* a, Node* b)
{
    return a && b ? (strcmp(a -> value, b -> value) == 0) && ComparisonNode(a -> left, b -> left) && ComparisonNode(a -> right , b -> right) : !a && !b;
}

void ConnectionNode(Node* left, Node* head, Node* right)
{
    head -> left = left;
    head -> right = right;
}

bool isdigitmore(char* str)// возможны проблемы с еxp!!
{
    if (str[0] == '0' || str[0] == 'e')
        return true;
    int a = atoi(str);
    if (!a)
        return false;
    return true;
}

//__________________________ПЛАНЫ :уменьшить код, (подчистить дерево после производной нормально), обработка данных (убрать умножение, скобки лишние, попробовать посчитать иксы)
int GetN1(char* string)
{
    int cursor = 0;
    int value1 = 0;
    //double value2 = 0;
    //double k = 1;
    while (isdigit(string[cursor]))
    {
        value1 = value1 * 10;
        value1 = value1 + string[cursor] - '0';
        cursor ++;
    }
    /*if (string[cursor] == '.')
    {
        cursor++;
        while (isdigit(string[cursor]))
        {
            k = k / 10;
            value2 = value2 * 10;
            value2 = value2 + (string[cursor] - '0');
            cursor ++;
        }
        value2 = value2 * k;
    }*/
    return value1 /*+ value2*/;
}

void SingnedValue(char* str)
{
    if (str[0] == '0')
        return;
    int a = atoi(str);
    if (!a)
    {
        printf("Ошибка SingnedValue\n");
        exit(100);
    }
    else
        sprintf(str, "%d", (-1)*a);
    printf("%s",str);
}