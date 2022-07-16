/// <reference types="react-scripts" />
//to import video
declare module '*.mp4' {
    const src: string;
    export default src;
  }