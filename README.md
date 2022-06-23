**About**

Beans is a new programming language which is being created with an initiative to make programming more fun, easier, efficient, available in various forms to develop in a multi-purpose way, etc. Generally programming languages are created for a certain purpose and later they are extended to be used as a general purpose programming language. Popular examples for this are:

**Perl** (Main use: string manipulation, Used as: General purpose programming language)<br>
**Java** (Main use: consumer electronic devices to communicate with each other, Used as: General purpose programming language)<br>
**Python** (Main use: bridge the gap between C and the shell, Used as: General purpose programming language), **etc**.

If we observe here, popular languages in this world are primarily developed for a particular purpose, later they are being converted into general purpose language. Main reason is that they are **extensible**. This feature enables them to expand their feet into various parts of software related technologies. This is why there are many general purpose programming languages available in market. Major drawback with these multiple availabilities is that whenever a project is being developed, as major languages can be used for multiple purposes, it becomes confusing to choose which language to use. For example, if your are developing a project which has three main concepts: Networking, AI and String manipulation and requirements: Fast, Efficient, Scalable, Cross-Platform Availability, etc, then, you need to wisely choose which language to use. Using different languages and merging them with **interoperability** may become costly at some point in some cases. Using only one language may cause difficulties to programmer or team while programming.

To avoid all these problems, I've come up with an idea to develop a programming language which will be available in various flavours just like linux kernel which can be used to develop multiple OS flavours of your own. Only similarity is linux is a kernel, initially developed with only 10,000 lines of code, this beans is an LL(1) parser (not so professionally coded), which can be used to develop a brand new programming language on top of it. This parser is being completely programmed by me, so that the code may not look professional, may not be completely optimized and so on. If you feel creepy after looking my code, I am extremely sorry for it.

**Guidelines**

This project is being open sourced under **GNU GPL license**. So you need to strictly adhere to all the rules under this license and also adhere to following rules and regulations:<br>
  
  - Must respect each and every developer view for this project and no criticism should be done.
  - Only source code files must be edited. Editing any other file(s) will be considered as violation of terms.
  - Should not release complete proprietary/free software developed with this code into market without open sourcing the actual source code along with the actual base source code.
  - Should not make changes to base grammar defined for this language. In case any changes made will be considered as violation of terms.
  - Should not sell code for anyone after forking or cloning the code. In case any sale made will be considered as violation of terms.
  - Modified code can be used for personal use and there is no need to open source the base and modified code. Also selling of personal use version of any form of this source code(code, executable, etc) are not allowed for sale or any other external purpose. Above kind of restricted usage will be considered as violation of terms.
  - Proper documentation or README file must be included in the repo in which you are open sourcing your code along with the base code if you've decided to launch this software into market. If not provided will be considered as violation of terms.
  - After creating a repo in which the both base and modified source codes are available, you need to add the repo link in the **links.yaml** file in this base repo, not in your new repo and the **links.yaml** file also should not be present in your new repo. In case of not performing any of the above mentioned tasks in this paragraph will be considered as violation of terms.

**Guide for files and folders in this repo**

LICENSE - This file contains information about the license under which this project is being open sourced.<br>
README.md - This file contains information regarding this project.<br>
Token.hpp - This header file which contains code for data structure which holds tokens generated by lexer.<br>
beans_lexer.l - This file contains flex code for lexer.<br>
beans_parser.cpp - This file contains hand-written parser code for this language.<br>
lex.yy.c - This file contains flex compiled C source code.<br>
links.yaml - Contains links for various repos which contains base and modified source codes along with this base repo.<br>
test.beans - This file contains test source code of beans language.<br>
Grammar - This file contains base grammar for beans language.<br>

**About telegram channel**

**Sponshorship request**
