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
            "\\title{Нахождение сложной производной}\n"
            "\\author{412 гр. Харламов Дмитрий}\n"
            "\\begin{document} \n"
            "\\maketitle\n"
            "\\section{Предисловие}\n"
            "Нахождение производных это очень важно для современного мира. "
            "Задачи по нахождению производных от больших и сложных функций "
            "повсеместно возникают у математиков,физиков, инженеров, и других. "
            "Особенно часто с данной задачей сталкиваются студенты\\cite{ku2}. "
            "В своей работе я хочу найти производную сложной функции, над "
            "нахождениемкоторой бьются лучшие умы человечества.\n"
            "\\section{Постановка задачи}\n"
            "Мы должны найти данную производную:\\newline\n$");

    math_to_TeX(tree);

    fprintf(gl_TeX, "$\\newline\n"
            "\\section{Решение задачи}\n"
            "Сначала упростим ее.Очевиден результат упрощения:\\newline\n$");

    tree = optim(tree);
    math_to_TeX(tree);

    fprintf(gl_TeX, "$\\newline\n"
            "Применив все правила нахождения "
            "производной, взятые из \\cite{ku1}, получим:\\newline\n$");

    math_to_TeX(tree);

    fprintf(gl_TeX, "$ = $");

    tree = differ(tree);
    math_to_TeX(tree);

    fprintf(gl_TeX, "$\\newline\n"
            "Производная найдена. Но для более удобного ее "
            "использования следует ее упростить. "
            "Приминив упращения, получим:\\newline\n$");

    math_to_TeX(tree);

    fprintf(gl_TeX, " $=$ ");

    tree = optim(tree);
    math_to_TeX(tree);

    fprintf(gl_TeX, "$\\newline\n"
            "\\section{Заключение}\n"
            "В итоге была найдена сложная производная:\\newline\n$");

    math_to_TeX(tree);


    fprintf(gl_TeX, "$\\newline\n"
            "Особая благодарность \\cite{ku3}\n"
            "\\begin{thebibliography}{9}\n"
            "\\bibitem[Google]{ku3}\n"
            "L. Page,\n"
            "S. Brin,\n"
            "Google,\n"
            "Stanford University,\n"
            "1998.\n"
            "\\bibitem{ku1}\n"
            "А. М. Тер-Крикоров,\n"
            "М. И. Шабунин,\n"
            "Курс математического анализа,\n"
            "2-е издание,\n"
            "Издательство Физматлит,\n"
            "2003.\n"
            "\\bibitem{ku2}\n"
            "Л. Д. Кудрявцев,\n"
            "А. Д. Кутасов,\n"
            "В. И. Чехлов,\n"
            "Сборник задач по математическому анализу 1 и 2 том,\n"
            "Издательство Физматлит,\n"
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




