#include "RunProg.hpp"

void run()
{
    char str_tmp[100] = "";
    bool tryAgain = false;
    bool user = true;
    while (user || tryAgain)
    {
        tryAgain = false;
        user = false;
        printf("\t\tWhat do you want to do? I can:\n\t\t1.Find the derivative of expression\n\t\t2.Calculation\n\t\t3.Exit\n");
        scanf(" %[^\n]", str_tmp);
        if (strcmp(str_tmp, unit_str) == 0)
            {
                user = true;
                printf("\tThe expression to read:\n");
                scanf(" %[^\n]", str_tmp);
                DescentParser k(str_tmp);
                Node* tree = new Node();
                tree = k.RecursiveDescentParser();
                FILE* texFile = OpenTex(tree);
                //tree -> ProcessingTree();
                if ((tree -> left) && (tree -> right))
                {
                    tree -> Conventer();
                    //Coventer = true;
                }
                tree -> ProcDerivative();
                tree -> ProcessingTree();
                tree -> BackConventer();
                tree -> ProcessingTree();
                tree -> ProcessingTree();
                tree -> ProcessingTree();
                tree -> ProcessingTree();
                tree -> BuildTexWithLaTex(texFile);
                CloseTex(texFile);
                system("pdflatex my_tex.tex");
                system("evince my_tex.pdf");
                openGraph(tree);
                system("evince my_graph.gv");
            }
        else if (strcmp(str_tmp, deuce_str) == 0)
        {
            user = true;
            printf("\t\tThe expression to read:\n");
            scanf(" %[^\n]", str_tmp);
            DescentParser1 str(str_tmp);
            printf("\t\tРезультат выражения : %g\n", str.RecursiveDescentParser1());
        }
        else if (strcmp(str_tmp, "3") == 0)
            user = false;
        else
        {
            printf("\t\tMade a mistake.Try again\n");
            user = false;
            tryAgain = true;
        }
        if (user)
            while (1)
            {
                printf("\t\tDo you want to calculate once again?\n");
                scanf(" %[^\n]", str_tmp);
                if (!tryAgain && strcmp(str_tmp, unit_str) == 0)
                {
                    //system("rm my_tex.pdf");
                    //system("rm my_tex.log");
                    //system("rm my_tex.aux");
                }
                if (strcmp(str_tmp, "no") == 0)
                {
                    user = false;
                    tryAgain = false;
                    break;
                }
                else if (strcmp(str_tmp, "yes") == 0)
                    break;
                else
                {
                    printf("\t\tMade a mistake.Try again\n");
                    tryAgain = true;
                    user = false;
                }
            }
    }
}

FILE* OpenTex(Node* tree)
{
    FILE *TexFile = fopen("my_tex.tex", "w");
    if (!TexFile)
    {
        fputs("Ошибка файла\n", stderr);
        exit(100);
    }
    fprintf(TexFile, "\\documentclass{article}\n\\title{Дифференцируем...}\n\\begin{document}\n\\author{students}\n\\maketitle\n\\fontsize{14}{8pt}\\selectfont\n\\begin{equation}\n[");
    tree -> BuildTexWithLaTex(TexFile);
    fprintf(TexFile, "]' =\n");
    fprintf(TexFile, "\\end{equation}\n");
    fprintf(TexFile,"\\begin{equation}\n");
    return TexFile;
}

void CloseTex(FILE* TexFile)
{
    fprintf(TexFile, "\n\\end{equation}\n\n\\end{document}");
    fclose(TexFile);
}
