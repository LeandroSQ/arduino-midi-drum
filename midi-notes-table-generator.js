let buffer = "";
const notes = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];

buffer += `<table>\n`;
buffer += `\t<tr>\n`;
buffer += `\t\t<th>Octave</th>\n`;
for (const note of notes) buffer += `\t\t<th>${note}</th>\n`;
buffer += `\t</tr>\n`;

let counter = 0;
for (let octave = 0; octave < 8; octave++) {
	buffer += `\t<tr>\n`;
	buffer += `\t\t<td><b>${octave}</b></td>\n`;

	for (const _ of notes) {
		buffer += `\t\t<td>${counter}</td>\n`;
		counter++;
	}
    
	buffer += `\t</tr>\n`;
}

buffer += `</table>`;

console.log(buffer);
