loop_even: ldc count
           ldnl 0
           brlz done_even
           ldc array
           add
           ldnl 0
           adc -1
           brlz found_even
           ldc count
           ldnl 0
           adc -1
           ldc count
           stnl 0
           br loop_even

found_even: ldc array
            add
            ldnl 0
            ldc first_even
            stnl 0
            br done_even

done_even: HALT

first_even: data 0
count:      data 9
array:      data 6
            data 5
            data 7
            data 10
            data 8
            data 5
            data 9
            data 3
            data 1
            data 4
