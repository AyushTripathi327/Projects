let arr = [];

const container = document.getElementById("container");

const newBtn = document.getElementById("newBtn");

const bubbleBtn = document.getElementById("bubbleBtn");

const mergeBtn = document.getElementById("mergeBtn");

newBtn.addEventListener("click", newArray);

bubbleBtn.addEventListener("click", bubbleSort);

mergeBtn.addEventListener("click", startMergeSort);

function newArray(){

    container.innerHTML = "";

    arr = [];

    for(let i = 0; i < 10; i++){

        let num = Math.floor(Math.random() * 200) + 20;

        arr.push(num);

        let bar = document.createElement("div");

        bar.classList.add("bar");

        bar.style.height = num + "px";

        bar.innerText = num;

        container.appendChild(bar);
    }
}

function wait(ms){

    return new Promise(resolve => setTimeout(resolve, ms));
}

async function bubbleSort(){

    document.getElementById("algo").innerText =
    "Bubble Sort";

    document.getElementById("time").innerText =
    "Time: O(n²)";

    document.getElementById("space").innerText =
    "Space: O(1)";

    let bars = document.getElementsByClassName("bar");

    for(let i = 0; i < arr.length; i++){

        for(let j = 0; j < arr.length - i - 1; j++){

            bars[j].style.background = "red";

            bars[j + 1].style.background = "red";

            await wait(300);

            if(arr[j] > arr[j + 1]){

                let temp = arr[j];

                arr[j] = arr[j + 1];

                arr[j + 1] = temp;

                bars[j].style.height = arr[j] + "px";

                bars[j].innerText = arr[j];

                bars[j + 1].style.height = arr[j + 1] + "px";

                bars[j + 1].innerText = arr[j + 1];
            }

            bars[j].style.background = "blue";

            bars[j + 1].style.background = "blue";
        }

        bars[arr.length - i - 1].style.background = "green";
    }
}

async function startMergeSort(){

    document.getElementById("algo").innerText =
    "Merge Sort";

    document.getElementById("time").innerText =
    "Time: O(n log n)";

    document.getElementById("space").innerText =
    "Space: O(n)";

    await mergeSort(0, arr.length - 1);

    let bars = document.getElementsByClassName("bar");

    for(let i = 0; i < bars.length; i++){

        bars[i].style.background = "green";
    }
}

async function mergeSort(left, right){

    if(left >= right){

        return;
    }

    let mid = Math.floor((left + right) / 2);

    await mergeSort(left, mid);

    await mergeSort(mid + 1, right);

    await merge(left, mid, right);
}

async function merge(left, mid, right){

    let temp = [];

    let i = left;

    let j = mid + 1;

    while(i <= mid && j <= right){

        if(arr[i] < arr[j]){

            temp.push(arr[i]);

            i++;
        }
        else{

            temp.push(arr[j]);

            j++;
        }
    }

    while(i <= mid){

        temp.push(arr[i]);

        i++;
    }

    while(j <= right){

        temp.push(arr[j]);

        j++;
    }

    let bars = document.getElementsByClassName("bar");

    for(let k = left; k <= right; k++){

        arr[k] = temp[k - left];

        bars[k].style.height = arr[k] + "px";

        bars[k].innerText = arr[k];

        bars[k].style.background = "orange";

        await wait(300);

        bars[k].style.background = "blue";
    }
}

newArray();