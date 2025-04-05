loop_min: ldc count
          ldnl 0
          brlz done_min
          ldc array
          add
          ldnl 0
          ldc mini
          ldnl 0
          sub
          brlz continue_min
          ldc mini
          ldnl 0
          add
          ldc mini
          stnl 0
          ldc count
          ldnl 0
          adc -1
          ldc count
          stnl 0
          br loop_min

continue_min: ldc count
              ldnl 0
              adc -1
              ldc count
              stnl 0
              br loop_min

done_min: HALT

mini:    data 0xFFFFFF
count:   data 9
array:   data 06
         data 4
         data 7
         data 10
         data 8
         data 5
         data 9
         data 2
         data 1
         data 3
