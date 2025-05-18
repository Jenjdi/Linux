#include <iostream>
#include <string>
#include <mysql/mysql.h>
const char *user = "connect";
const char *passwd = "123456";
const char *db = "conn";
const char *host = "localhost";
const unsigned int port = 3306;
int main()
{
    MYSQL *my = mysql_init(nullptr);
    if (nullptr == my)
    {
        std::cerr << "init failed" << std::endl;
        return 1;
    }
    if (nullptr == mysql_real_connect(my, host, user, passwd, db, port, nullptr, 0))
    {
        std::cerr << "connect failed" << std::endl;
        return 1;
    }
    std::cout << "connect success" << std::endl;
    mysql_set_character_set(my, "utf8");
    std::string line;

    do
    {
        std::cout << "mysql> ";
        if (!getline(std::cin, line) || line == "quit")
        {
            std::cout << "bye" << std::endl;
            break;
        }
        int n = mysql_query(my, line.c_str());
        if (n == 0)
        {
            std::string line1=line.substr(0, 6);
            if (line1 == "select")
            {
                MYSQL_RES *res = mysql_store_result(my);
                my_ulonglong row = mysql_num_rows(res); // 行数
                unsigned int field = mysql_num_fields(res);       // 列数
                MYSQL_FIELD *fieldname = mysql_fetch_fields(res); // 列名
                for (int i = 0; i < row; i++)
                {
                    MYSQL_ROW rowname = mysql_fetch_row(res);
                    for (int j = 0; j < field; j++)
                    {
                        std::cout << rowname[j] << "\t";
                    }
                    std::cout << std::endl;
                }
            }
            std::cout << "succsee" << std::endl;
        }
        else
        {
            std::cerr << "failed" << std::endl;
        }
    } while (true);
    mysql_close(my);
}
