using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    public class Singleton
    {
        private static Singleton instance;
        public static Singleton Instance
        {
            get
            {
                if(instance is null)
                {
                    instance = new Singleton();
                    Console.WriteLine("Create new instance");
                }
                else
                {
                    Console.WriteLine("Instance existed. Return");
                }
                return instance;
            }
        }
    }
}
