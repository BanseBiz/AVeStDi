# Autonomous Vehicle Status Distributer (AVeStDi) #
## TCP-Socket on 6156: ##
#### VERS: returns date of build as string ####
Answer:
    AVeSti Daemon Version: Mar  8 2024 14:59:30
#### QUIT: closes connection ####
Answer:
    EXIT bye
#### LIST: returns json array of all known AVs #####
Answer:
    [{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603001","timestamp":"105709519199112"",position":[53.11933910,8.19172210,8.400]},{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603002","timestamp":"105709519203016"",position":[53.11562820,8.19115920,8.400]},{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603003","timestamp":"105709519204104"",position":[53.11734130,8.18872230,8.400]}]
#### PUTS {json string}: pushes a given json string to publish or update AV ####
Request:
    PUTS {"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603001","date":"20220227","time":"014307692","position":[53.1193391, 8.1917221,8.4]}
Answer:
    OK
#### PUTL {json string}: like PUTS, but server answers with json array of all known AVs, including distance and direction towards the pushed AV ####
Request: PUTL {"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603001","date":"20220227","time":"014307692","position":[53.1193391, 8.1917221,8.4]}
Answer:
    [{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603002","timestamp":"105709519203016"",position":[53.11562820,8.19115920,8.400]",distance":414.6966,"direction":5.2142},{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603003","timestamp":"105709519204104"",position":[53.11734130,8.18872230,8.400]",distance":299.6184,"direction":42.0924}]
      
## WEB-Socket on 8096: ##
GET on /*: returns data of build as string
PUT <json string> on /*: pushes a given json string to publish or update AV
GET on /list: returns json array of all known AVs
PUT <json string> on /list: like PUT on /*, but server answers with json array of all known AVs, including distance and direction towards the pushed AV
        
    Examples:
        CMD: curl http://127.0.0.1:8096     
          Answer: AVeSti Daemon Version: Mar  8 2024 14:59:30
        CMD: curl http://127.0.0.1:8096/list 
          Answer: [{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603008","timestamp":"124725582230248","position":[53.12178430,8.70339620,8.400],"std_dev":[0.98,1.70,2.12],"orientation":[0.0000,0.0000,0.0000],"velocity":[0.0000,0.0000,0.0000]},{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603009","timestamp":"124725582229176","position":[53.12178430,8.70339620,8.400],"std_dev":[0.98,1.70,2.12],"orientation":[0.0000,0.0000,0.0000],"velocity":[0.0000,0.0000,0.0000]}]
        CMD: curl -X PUT -H "Content-Type: application/json" -d '{"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603008","date":"20220227","time":"014307692","position":[53.1217843,8.7033962,8.4],"std_dev":[0.98,1.70,2.12],"velocity":0.055,"course":0.000,"mode":5,"sat_cnt":12}' http://127.0.0.1:8096
          Answer: OK
      CMD: curl -X PUT -H "Content-Type: application/json" -d '{"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603008","date":"20220227","time":"014307692","position":[53.1217843,8.7033962,8.4],"std_dev":[0.98,1.70,2.12],"velocity":0.055,"course":0.000,"mode":5,"sat_cnt":12}' http://127.0.0.1:8096/list
    Answer: [{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603009","timestamp":"124725582229176","position":[53.12178430,8.70339620,8.400],"std_dev":[0.98,1.70,2.12],"orientation":[0.0000,0.0000,0.0000],"velocity":[0.0000,0.0000,0.0000],"distance":0.0000,"direction":180.0000}]
