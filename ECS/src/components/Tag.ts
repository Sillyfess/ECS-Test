import { Component } from '../Component';

export class Tag extends Component {
    public name: string;

    constructor(name: string) {
        super();
        this.name = name;
    }

    public clone(): Tag {
        return new Tag(this.name);
    }
}