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
        ConnectionNode(NewNodeValueRightLeft, NewNodeOperand