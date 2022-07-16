import { useState, useEffect } from 'react';

const Timer = (props:any) => {
    const initialSeconds=0, initialMinute = 0;
    const [ minutes, setMinutes ] = useState(initialMinute);
    const [seconds, setSeconds ] =  useState(initialSeconds);
    useEffect(()=>{
    let myInterval = setInterval(() => {
        if (props == 0){
        setSeconds(seconds);
        }else{
            if(seconds == 60){
                setSeconds(0);
                setMinutes(minutes+1);}
            else{
                setSeconds(seconds + 1);}
        }
        }, 1000)
        return ()=> {
            clearInterval(myInterval);
        };
    });
    return (seconds)
}
export default Timer;