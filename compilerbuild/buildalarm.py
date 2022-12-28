#!/bin/python3

import sys
import subprocess
import shlex
import pathlib
import re
import argparse
#from pygame import mixer


def poll_process(process):
    while True:
        output = process.stdout.readline()
        return_code = process.poll()
        if return_code is not None: 
            if output:
                yield output
            yield return_code
            break
        elif output: 
            yield output

def print_process_update(
            process, 
            output_file, 
            output_character_limit = 10000, 
            output_line_limit = 100, 
            output_file_line_limit = -1, 
            file_front_trunc = None, 
            file_back_trunc = None, 
            console_front_trunc = None, 
            console_back_trunc = None, 
            regex_filter = None, 
            truncation_indicator = "<..._TRUNCATED_...>"
        ): 
    regex_filter = re.compile(regex_filter) if regex_filter else None
    output_lines = 0
    output_file_lines = 0
    diagnostic_output_to_many_lines = False
    diagnostic_output_file_to_many_lines = False
    for output in poll_process(process):
        if not (type(output) is int): 
            output = output.decode('utf-8')
            if output_lines < output_line_limit: 
                output_truncated = (output[ : console_front_trunc] + truncation_indicator + output[console_back_trunc : ] \
                        if len(output) > console_front_trunc else output) \
                        if console_front_trunc else output
                if len(output_truncated) < output_character_limit: 
                    if not regex_filter: 
                        output_lines += 1
                        print(output_truncated)
                    elif regex_filter.search(output): 
                        output_lines += 1
                        print(output_truncated)
                else:
                    print("\nLine too long as per set character limit (" + str(output_character_limit) + ") check output file for full log.")
            elif diagnostic_output_to_many_lines == False:
                    print("\nToo many output lines per set limit (" + str(output_file_line_limit) + ")" 
                            + (" sending to log file, check it for a full log.\n" if diagnostic_output_file_to_many_lines == False else "\n")
                        )
                    diagnostic_output_to_many_lines = True
            output_truncated = (output[ : file_front_trunc] + truncation_indicator + output[file_back_trunc : ] \
                    if len(output) > file_front_trunc else output) \
                    if file_front_trunc else output
            if output_file_line_limit > -1 and output_file_lines < output_file_line_limit: 
                output_file.write(output_truncated)
                output_file_lines += 1
            elif output_file_line_limit <= -1: 
                output_file.write(output_truncated)
            elif diagnostic_output_file_to_many_lines == False: 
                print("\nFile log too large acccording to set limit (" + str(output_file_line_limit) + ") process will continue but will not log\n")
                diagnostic_output_file_to_many_lines = True
        else: 
            print("Process terminated with code: ", output)

def not_none_if_index(list, index):
    return (True if list[index] else False) if len(list) > index else False

def element_if_not_none(list, index): 
    return list[index] if list else None

def main():
    parser = argparse.ArgumentParser(description="Logs the output of a long command while letting you see its progress.")
    parser.add_argument("command", type=str, nargs=1, help="The command you wish to run + log")
    parser.add_argument("--output_character_limit", "-ocl", type=int, nargs=1, 
            help = "Limits lines printed too the console to less than this number of characters per line", 
            default = 10000
        )
    parser.add_argument("--output_line_limit", "-oll", type=int, nargs=1, 
            help = "Limits the number of lines printed too the console (but not log file.", 
            default = 100
        )
    parser.add_argument("--output_file_line_limit", "-ofll", type=int, nargs=1, 
            help = "Limits the number of lines actuall output to the log file.", 
            default = 10000
        )
    parser.add_argument("--output", "-o", type=pathlib.Path, nargs=1, 
            help="Logfile where output will go.", 
            default = pathlib.Path.cwd() / "output.cpp"
        )
    parser.add_argument("--print_regex", "-pr", type=str, nargs=1, 
            help = "A regex which filters only the output you want too see in the console (but not file)"
        )
    parser.add_argument("--output_truncate", "-ot", type=int, nargs=2, 
            help = "2 integers, how may characters to keep from the front, and back?"
        )
    parser.add_argument("--output_file_truncate", "-oft", type=int, nargs=2, 
            help = "2 integers, how may characters to keep from the front, and back in lines sent too the log file?"
        )
    arguments = parser.parse_args()
    process = subprocess.Popen(shlex.split(arguments.command[0]), shell=False, stdout=subprocess.PIPE)
    output_file_name = str(arguments.output)
    output_line_character_limit = arguments.output_character_limit
    output_line_limit = arguments.output_line_limit
    output_file_line_limit = arguments.output_file_line_limit
    with open(output_file_name, 'w') as log: 
        print_process_update(
                process, 
                log, 
                output_line_character_limit, 
                output_line_limit, 
                output_file_line_limit,
                element_if_not_none(arguments.output_file_truncate, 0), 
                element_if_not_none(arguments.output_file_truncate, 1), 
                element_if_not_none(arguments.output_truncate, 0), 
                element_if_not_none(arguments.output_truncate, 1), 
                element_if_not_none(arguments.print_regex, 0)
            )
#    try:
#        mixer.init()
#        alarm = mixer.Sound("alarm.wav")
#        alarm.play()
#    except:
#        print("Coulden't ring alarm: No audio device found, sorry!")

if __name__ == "__main__":
    main()


