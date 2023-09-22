import http from 'k6/http';
import { sleep } from 'k6';



export default function () {

    const res = http.get('https://google.com');

    for (const p in res.headers) 
    {
        if (res.headers.hasOwnProperty(p))
        {
            console.log(p + ' : ' + res.headers[p]);
        }
  }
  sleep(1);

}
