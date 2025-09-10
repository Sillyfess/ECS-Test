"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Tag = void 0;
const Component_1 = require("../Component");
class Tag extends Component_1.Component {
    constructor(name) {
        super();
        this.name = name;
    }
    clone() {
        return new Tag(this.name);
    }
}
exports.Tag = Tag;
//# sourceMappingURL=Tag.js.map