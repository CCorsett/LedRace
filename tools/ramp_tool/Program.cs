using System;
using System.Collections.Generic;

namespace ramp_tool
{
    class Program
    {
        public static int get_values()
        {
            int val = -1;
            string line = String.Empty;
            do
            {
                line = Console.ReadLine();
                if (int.TryParse(line, out val))
                {

                }
                else
                {
                    Console.WriteLine("Give me a positive integer please :)");
                }
            }
            while (val != -1);
            return (val);
        }
        static void Main(string[] args)
        {
            Console.WriteLine("Howdy!! I'm Rampy");
            Console.WriteLine("Give me the total number of led strip: ");
            int led_num = get_values();
            Console.WriteLine("Give me the high of the ramp: ");
            int high = get_values();
            Console.WriteLine("Give me the start led of the ramp: ");
            int ramp_start_led = get_values();
            Console.WriteLine("Give me the center led of the ramp: ");
            int ramp_center_led = get_values();
            Console.WriteLine("Give me the end led of the ramp: ");
            int ramp_end_led = get_values();

            List<int> gravity = new List<int>();
            for (int i = 0; i < led_num; i++)
            {
                gravity.Add(127);
            }

            Console.WriteLine("Hill ramp values: ");
            for (int j = 0; j < (ramp_center_led - ramp_start_led); j++)
            {
                gravity[ramp_start_led + j] = (int)(127 - j * ((float)high / (ramp_center_led - ramp_start_led)));
                Console.WriteLine(gravity[ramp_start_led + j]);
            }
        }
    }
}
