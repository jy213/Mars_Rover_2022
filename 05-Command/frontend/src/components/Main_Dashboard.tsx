import Coordinates from './Coordinates'
import Map from './Rover_Map'
import Controller from './Mannual'
import Database from './Database'
import Title from './Title'
import Signal from './Signal'
import './Main_Dashboard.css'

export default function Main_Dashboard () {
  
    return (

      <div className="container">
        <div><Title/></div>
        <div><Signal/> </div>
        <div className="item"><Coordinates/></div>
        <div className="map"><Map/></div>
        <div ><Database/></div>
        <div className="mannual_container">
          <div><Controller/></div>
        </div>
      </div>
    )
  }