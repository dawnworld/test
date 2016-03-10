using System;
using System.Text.RegularExpressions;


namespace HelloWorld
{
    class Program
    {
        /// <summary>
        /// Main方法：控制台应用程序的入口
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            Program p = new Program();
            // 向控制台输出一句话
            Console.WriteLine("Hello,world!");
            // 窗口暂停，等待按键
            // Console.ReadKey();
            // p.test_split_str();
            // p.test_str_trim();
            p.test_regex();
        }

        public void test_split_str()
        {
            Console.WriteLine(" test split string");
            string s = "a'';";
            string[] result = s.Split(';');
            foreach(string str in result)
            {
                Console.WriteLine(str);
            }
        }

        public void test_str_trim()
        {
            string s = "varchar(20)";

            Console.WriteLine(s.TrimEnd('('));
        }

        public void test_regex()
        {
            // string text = "varchar(20)";
            string text = "u2d(@a_name())";
            // string text = "insert into dst_test_tbl(id, name) values ('@id', '@name');";
            // string pattern = @"([^\(]*)";
            // string pattern = @"[^\d]*(\d*)";
            // string pattern = @"\((\d*)\)";
            string pattern = @"\((.*)\)";
            // string pattern = @"@id";
            // string replace = "{0}";
            // Regex rgx = new Regex(pattern);
            // string result = rgx.Replace(text, replace);

            // string new_res = string.Format(result, "1");
            // Console.WriteLine(new_res);

            MatchCollection matches = Regex.Matches(text, pattern);
            Console.WriteLine(matches[0].Length);
            Console.WriteLine(matches[0].Groups[1].Value);
            
            // foreach (Match match in matches)
            // {
            //     GroupCollection groups = match.Groups;
            //     Console.WriteLine(groups[0].Value);
            // }
        }
    }
}
