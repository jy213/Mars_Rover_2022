import { useState, useEffect } from 'react';
const Battery = (props:any) => {
    const initialSeconds = 90;
    const [seconds, setSeconds ] =  useState(initialSeconds);
    useEffect(()=>{
    let myInterval = setInterval(() => {
            setSeconds(seconds - 1);}
        , 3000)
        return ()=> {
            clearInterval(myInterval);
        };
    });
    return (seconds)
}
export default Battery;