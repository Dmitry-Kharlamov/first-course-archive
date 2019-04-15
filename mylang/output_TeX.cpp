#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include "tree.h"
#include "differentiation.h"
#include "output_TeX.h"
#include "errors.h"


FILE* gl_TeX = fopen("derivative.tex", "w");
//int isTeXoutput = 0;


int output_TeX(node_t* tree, char* filename)
{
    if(filename) gl_TeX = fopen(filename, "w");

    setlocale(LC_ALL, "russian_russia.1251");

    fprintf(gl_TeX,
            "\\documentclass[12pt]{article}\n"
            "\\usepackage[russian]{babel}\n"
            "\\title{���������� ������� �����������}\n"
            "\\author{412 ��. �������� �������}\n"
            "\\begin{document} \n"
            "\\maketitle\n"
            "\\section{�����������}\n"
            "���������� ����������� ��� ����� ����� ��� ������������ ����. "
            "������ �� ���������� ����������� �� ������� � ������� ������� "
            "����������� ��������� � �����������,�������, ���������, � ������. "
            "�������� ����� � ������ ������� ������������ ��������\\cite{ku2}. "
            "� ����� ������ � ���� ����� ����������� ������� �������, ��� "
            "������������������ ������ ������ ��� ������������.\n"
            "\\section{���������� ������}\n"
            "�� ������ ����� ������ �����������:\\newline\n$");

    math_to_TeX(tree);

    fprintf(gl_TeX, "$\\newline\n"
            "\\section{������� ������}\n"
            "������� �������� ��.�������� ��������� ���������:\\newline\n$");

    tree = optim(tree);
    math_to_TeX(tree);

    fprintf(gl_TeX, "$\\newline\n"
            "�������� ��� ������� ���������� "
            "�����������, ������ �� \\cite{ku1}, �������:\\newline\n$");

    math_to_TeX(tree);

    fprintf(gl_TeX, "$ = $");

    tree = differ(tree);
    math_to_TeX(tree);

    fprintf(gl_TeX, "$\\newline\n"
            "����������� �������. �� ��� ����� �������� �� "
            "������������� ������� �� ���������. "
            "�������� ���������, �������:\\newline\n$");

    math_to_TeX(tree);

    fprintf(gl_TeX, " $=$ ");

    tree = optim(tree);
    math_to_TeX(tree);

    fprintf(gl_TeX, "$\\newline\n"
            "\\section{����������}\n"
            "� ����� ���� ������� ������� �����������:\\newline\n$");

    math_to_TeX(tree);


    fprintf(gl_TeX, "$\\newline\n"
            "������ ������������� \\cite{ku3}\n"
            "\\begin{thebibliography}{9}\n"
            "\\bibitem[Google]{ku3}\n"
            "L. Page,\n"
            "S. Brin,\n"
            "Google,\n"
            "Stanford University,\n"
            "1998.\n"
            "\\bibitem{ku1}\n"
            "�. �. ���-��������,\n"
            "�. �. �������,\n"
            "���� ��������������� �������,\n"
            "2-� �������,\n"
            "������������ ���������,\n"
            "2003.\n"
            "\\bibitem{ku2}\n"
            "�. �. ���������,\n"
            "�. �. �������,\n"
            "�. �. ������,\n"
            "������� ����� �� ��������������� ������� 1 � 2 ���,\n"
            "������������ ���������,\n"
            "2003.\n"
            "\\end{thebibliography}\n"
            "\\end{document}\n");


    return 0;

}

int math_to_TeX(node_t* node)
{
    switch (node->val_type)
    {
    case STR:
    {
        fprintf(gl_TeX, "%s", node->val.s);
        if (!strcmp(node->val.s, "sin") ||
            !strcmp(node->val.s, "cos"))
            {
                fprintf(gl_TeX, "(");
                math_to_TeX(node->left);
                fprintf(gl_TeX, ")");
            }
    }
    break;

    case OPER:
        switch (node->val.s[0])
        {

        case'+':
        {
            math_to_TeX(node->left );
            fprintf(gl_TeX, " + ");
            math_to_TeX(node->right);
        }
        break;

        case'-':
        {
            math_to_TeX(node->left );
            fprintf(gl_TeX, " - ");
            math_to_TeX(node->right);
        }
        break;

        case'*':
        {
            math_to_TeX(node->left );
            fprintf(gl_TeX, " \\cdot ");
            math_to_TeX(node->right);
        }
        break;

        case'/':
        {
            fprintf(gl_TeX, "\\frac{");
            math_to_TeX(node->left );
            fprintf(gl_TeX, "}{");
            math_to_TeX(node->right);
            fprintf(gl_TeX, "}");
        }
        break;

        case'\'':
        {
            fprintf(gl_TeX, "(");
            math_to_TeX(node->left );
            fprintf(gl_TeX, ")'_");
            math_to_TeX(node->right);
        }
        break;

        }
    break;

    case NUMB:
        if (node->val.d < 0)
            fprintf(gl_TeX, "(%d)", node->val.d);
        else
            fprintf(gl_TeX, "%d", node->val.d);

    break;

    case VAR:
        fprintf(gl_TeX, "%s", node->val.s);
    break;

    }


    return 0;
}




